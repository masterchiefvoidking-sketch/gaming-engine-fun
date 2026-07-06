#include <algorithm>
#include <cmath>
#include <eve/render/inspection_camera.hpp>

namespace eve::render {

namespace {
constexpr f32 kPi = 3.14159265f;
f32 deg_to_rad(f32 degrees) {
    return degrees * kPi / 180.0f;
}
} // namespace

void InspectionCamera::set_mode(CameraMode mode) {
    mode_ = mode;
}

void InspectionCamera::set_focus(const math::Vec3& focus) {
    focus_ = focus;
}

void InspectionCamera::set_fov(f32 fov_y_degrees) {
    fov_y_degrees_ = std::clamp(fov_y_degrees, 10.0f, 120.0f);
}

void InspectionCamera::add_preset(CameraPreset preset) {
    presets_[preset.id] = std::move(preset);
}

void InspectionCamera::apply_preset(std::string_view preset_id) {
    const auto it = presets_.find(std::string(preset_id));
    if (it == presets_.end()) {
        return;
    }
    position_ = it->second.position;
    focus_ = it->second.focus;
    fov_y_degrees_ = it->second.fov_y_degrees;
}

void InspectionCamera::begin_cinematic_path(std::vector<math::Vec3> path, f32 duration_seconds) {
    cinematic_path_ = std::move(path);
    cinematic_duration_ = std::max(duration_seconds, 0.01f);
    cinematic_elapsed_ = 0.0f;
    mode_ = CameraMode::Cinematic;
}

void InspectionCamera::update(f32 delta_seconds, const OrbitCameraInput& input) {
    if (transitioning_) {
        transition_elapsed_ += delta_seconds;
        const f32 t = std::clamp(transition_elapsed_ / transition_duration_, 0.0f, 1.0f);
        const f32 smooth = t * t * (3.0f - 2.0f * t);
        position_ = transition_start_pos_ * (1.0f - smooth) + transition_end_pos_ * smooth;
        focus_ = transition_start_focus_ * (1.0f - smooth) + transition_end_focus_ * smooth;
        if (t >= 1.0f) {
            transitioning_ = false;
        }
        return;
    }

    if (input.set_focus) {
        focus_ = input.focus_point;
    }

    switch (mode_) {
    case CameraMode::Orbit:
    case CameraMode::Photo:
    case CameraMode::Wardrobe:
    case CameraMode::Mirror:
    case CameraMode::BodyRegionFocus:
        update_orbit(input);
        break;
    case CameraMode::Cinematic:
        update_cinematic(delta_seconds);
        break;
    case CameraMode::Free:
        position_ = position_ + math::Vec3{input.pan_delta.x, input.pan_delta.y, 0.0f};
        break;
    case CameraMode::FirstPerson:
        yaw_ += input.orbit_delta_yaw;
        pitch_ = std::clamp(pitch_ + input.orbit_delta_pitch, -1.4f, 1.4f);
        break;
    case CameraMode::ThirdPerson:
    case CameraMode::Shoulder:
        update_orbit(input);
        if (mode_ == CameraMode::Shoulder) {
            position_.x += 0.45f;
        }
        break;
    }
}

math::Mat4 InspectionCamera::view_matrix() const {
    if (mode_ == CameraMode::FirstPerson) {
        const f32 cy = std::cos(pitch_);
        const math::Vec3 forward{std::sin(yaw_) * cy, std::sin(pitch_), std::cos(yaw_) * cy};
        return math::Mat4::look_at(position_, position_ + forward, {0.0f, 1.0f, 0.0f});
    }
    return math::Mat4::look_at(position_, focus_, {0.0f, 1.0f, 0.0f});
}

math::Mat4 InspectionCamera::projection_matrix(f32 aspect) const {
    return math::Mat4::perspective(deg_to_rad(fov_y_degrees_), aspect, near_z_, far_z_);
}

void InspectionCamera::update_orbit(const OrbitCameraInput& input) {
    yaw_ += input.orbit_delta_yaw;
    pitch_ = std::clamp(pitch_ + input.orbit_delta_pitch, -1.4f, 1.4f);
    distance_ = std::clamp(distance_ - input.zoom_delta, 0.5f, 12.0f);
    focus_ = focus_ + math::Vec3{input.pan_delta.x, input.pan_delta.y, 0.0f};

    const f32 cy = std::cos(pitch_);
    const math::Vec3 offset{std::sin(yaw_) * cy * distance_, std::sin(pitch_) * distance_ + 1.5f,
                            std::cos(yaw_) * cy * distance_};
    position_ = focus_ + offset;
}

void InspectionCamera::update_cinematic(f32 delta_seconds) {
    if (cinematic_path_.size() < 2) {
        return;
    }
    cinematic_elapsed_ += delta_seconds;
    const f32 t = std::clamp(cinematic_elapsed_ / cinematic_duration_, 0.0f, 1.0f);
    const f32 scaled = t * static_cast<f32>(cinematic_path_.size() - 1);
    const std::size_t index = static_cast<std::size_t>(scaled);
    const std::size_t next = std::min(index + 1, cinematic_path_.size() - 1);
    const f32 local_t = scaled - static_cast<f32>(index);
    position_ = cinematic_path_[index] * (1.0f - local_t) + cinematic_path_[next] * local_t;
}

void InspectionCamera::focus_body_region(const math::Vec3& center, const math::Vec3& offset) {
    mode_ = CameraMode::BodyRegionFocus;
    focus_ = center;
    position_ = center + offset + math::Vec3{0.0f, 0.0f, 1.2f};
    distance_ = 1.2f;
}

void InspectionCamera::transition_to(const math::Vec3& position, const math::Vec3& focus, f32 duration_seconds) {
    transition_start_pos_ = position_;
    transition_end_pos_ = position;
    transition_start_focus_ = focus_;
    transition_end_focus_ = focus;
    transition_duration_ = std::max(duration_seconds, 0.01f);
    transition_elapsed_ = 0.0f;
    transitioning_ = true;
}

void InspectionCamera::set_content_rating_lockout(ContentRatingLockout lockout) {
    rating_lockout_ = lockout;
}

bool InspectionCamera::is_locked_out(ContentRatingLockout required_rating) const {
    return static_cast<u8>(required_rating) > static_cast<u8>(rating_lockout_);
}

} // namespace eve::render
