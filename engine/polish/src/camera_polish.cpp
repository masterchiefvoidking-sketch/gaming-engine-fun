#include <eve/polish/camera_polish.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <algorithm>
#include <cmath>
#include <nlohmann/json.hpp>

namespace eve::polish {

namespace {

render::CameraPreset make_preset(std::string id, std::string label, math::Vec3 pos, math::Vec3 focus,
                                 f32 fov) {
    render::CameraPreset preset;
    preset.id = std::move(id);
    preset.label = std::move(label);
    preset.position = pos;
    preset.focus = focus;
    preset.fov_y_degrees = fov;
    return preset;
}

} // namespace

bool CameraPolish::load_room_presets(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        presets_["mirror"] = make_preset("mirror", "Mirror", {0.0f, 1.5f, 2.2f}, {0.0f, 1.4f, 0.0f},
                                         32.0f);
        presets_["sofa_conversation"] =
            make_preset("sofa_conversation", "Sofa", {-1.5f, 1.4f, 2.5f}, {0.0f, 1.2f, 0.0f}, 38.0f);
        presets_["dining_table"] =
            make_preset("dining_table", "Dining", {2.0f, 1.6f, 2.0f}, {0.0f, 1.1f, 0.0f}, 40.0f);
        presets_["bedroom_goodnight"] = make_preset("bedroom_goodnight", "Goodnight",
                                                    {0.5f, 1.5f, 2.8f}, {0.0f, 1.0f, 0.0f}, 34.0f);
        presets_["bathroom_privacy"] = make_preset("bathroom_privacy", "Bathroom",
                                                   {0.0f, 1.6f, smoothing_.privacy_safe_distance},
                                                   {0.0f, 1.5f, 0.0f}, 30.0f);
        presets_["photo_mode"] =
            make_preset("photo_mode", "Photo", {0.0f, 1.55f, 3.5f}, {0.0f, 1.5f, 0.0f}, 28.0f);
        presets_["full_body"] =
            make_preset("full_body", "Full Body", {0.0f, 1.5f, 3.2f}, {0.0f, 1.2f, 0.0f}, 35.0f);
        presets_["face"] =
            make_preset("face", "Face", {0.0f, 1.65f, 1.2f}, {0.0f, 1.62f, 0.0f}, 25.0f);
        return true;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            render::CameraPreset preset;
            preset.id = entry.value("id", "");
            preset.label = entry.value("label", preset.id);
            preset.position = {entry.value("px", 0.0f), entry.value("py", 1.5f),
                               entry.value("pz", 3.0f)};
            preset.focus = {entry.value("fx", 0.0f), entry.value("fy", 1.5f),
                            entry.value("fz", 0.0f)};
            preset.fov_y_degrees = entry.value("fov", 35.0f);
            presets_[preset.id] = preset;
            if (entry.contains("room")) {
                RoomCameraLimits limits;
                limits.room_id = entry.value("room", "");
                limits.min_distance = entry.value("min_distance", 1.2f);
                limits.max_distance = entry.value("max_distance", 6.0f);
                room_limits_[limits.room_id] = limits;
            }
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void CameraPolish::set_smoothing(const CameraSmoothingSettings& settings) {
    smoothing_ = settings;
}

void CameraPolish::set_active_room(std::string_view room_id) {
    active_room_ = std::string(room_id);
}

f32 CameraPolish::smooth_damp(f32 current, f32 target, f32& velocity, f32 delta) const {
    const f32 omega = target > current ? smoothing_.acceleration : smoothing_.deceleration;
    const f32 t = 1.0f - std::exp(-omega * delta);
    velocity = (target - current) / std::max(delta, 0.001f);
    return current + (target - current) * t;
}

math::Vec3 CameraPolish::clamp_position(const math::Vec3& pos) const {
    const RoomCameraLimits* limits = active_limits();
    if (limits == nullptr) {
        return pos;
    }
    math::Vec3 clamped = pos;
    clamped.x = std::clamp(clamped.x, limits->bounds_min.x, limits->bounds_max.x);
    clamped.y = std::clamp(clamped.y, limits->bounds_min.y, limits->bounds_max.y);
    clamped.z = std::clamp(clamped.z, limits->bounds_min.z, limits->bounds_max.z);
    return clamped;
}

void CameraPolish::apply_smooth_input(render::InspectionCamera& camera, f32 delta_seconds,
                                    const interaction::InputFrame& input) {
    render::OrbitCameraInput smoothed;
    smoothed.orbit_delta_yaw =
        smooth_damp(0.0f, input.orbit_delta_yaw, yaw_velocity_, delta_seconds);
    smoothed.orbit_delta_pitch =
        smooth_damp(0.0f, input.orbit_delta_pitch, pitch_velocity_, delta_seconds);
    smoothed.zoom_delta = smooth_damp(0.0f, input.zoom_delta, zoom_velocity_, delta_seconds);
    smoothed.pan_delta = input.pan_delta;
    camera.update(delta_seconds, smoothed);

    math::Vec3 pos = camera.position();
    const math::Vec3 focus = camera.focus();
    const math::Vec3 to_camera = pos - focus;
    const f32 dist = std::sqrt(to_camera.x * to_camera.x + to_camera.y * to_camera.y +
                               to_camera.z * to_camera.z);
    const RoomCameraLimits* limits = active_limits();
    if (limits != nullptr) {
        if (dist < limits->min_distance || dist > limits->max_distance) {
            const f32 clamped_dist = std::clamp(dist, limits->min_distance, limits->max_distance);
            const f32 scale = clamped_dist / std::max(dist, 0.001f);
            pos = focus + math::Vec3{to_camera.x * scale, to_camera.y * scale, to_camera.z * scale};
        }
    }
    pos = clamp_position(pos);
    camera.transition_to(pos, focus, 0.15f);
}

bool CameraPolish::apply_room_preset(render::InspectionCamera& camera, std::string_view preset_id) {
    const auto it = presets_.find(std::string(preset_id));
    if (it == presets_.end()) {
        return false;
    }
    camera.apply_preset(preset_id);
    camera.transition_to(it->second.position, it->second.focus, 0.4f);
    return true;
}

bool CameraPolish::focus_face(render::InspectionCamera& camera) {
    return apply_room_preset(camera, "face");
}

bool CameraPolish::focus_full_body(render::InspectionCamera& camera) {
    return apply_room_preset(camera, "full_body");
}

bool CameraPolish::focus_mirror(render::InspectionCamera& camera) {
    camera.set_mode(render::CameraMode::Mirror);
    return apply_room_preset(camera, "mirror");
}

bool CameraPolish::focus_sofa_conversation(render::InspectionCamera& camera) {
    return apply_room_preset(camera, "sofa_conversation");
}

bool CameraPolish::focus_dining_table(render::InspectionCamera& camera) {
    return apply_room_preset(camera, "dining_table");
}

bool CameraPolish::focus_bedroom_goodnight(render::InspectionCamera& camera) {
    return apply_room_preset(camera, "bedroom_goodnight");
}

bool CameraPolish::focus_bathroom_privacy_safe(render::InspectionCamera& camera) {
    camera.set_mode(render::CameraMode::BodyRegionFocus);
    return apply_room_preset(camera, "bathroom_privacy");
}

bool CameraPolish::focus_photo_mode(render::InspectionCamera& camera) {
    camera.set_mode(render::CameraMode::Photo);
    camera.depth_of_field_mutable().enabled = true;
    return apply_room_preset(camera, "photo_portrait");
}

const RoomCameraLimits* CameraPolish::active_limits() const {
    const auto it = room_limits_.find(active_room_);
    if (it == room_limits_.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace eve::polish
