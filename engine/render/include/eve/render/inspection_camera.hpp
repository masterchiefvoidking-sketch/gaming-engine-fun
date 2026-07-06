#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::render {

enum class CameraMode : u8 {
    Orbit = 0,
    Free,
    Shoulder,
    FirstPerson,
    ThirdPerson,
    Cinematic,
    Photo,
    Wardrobe,
    Mirror,
    BodyRegionFocus,
};

enum class ContentRatingLockout : u8 {
    None = 0,
    Mature,
    AdultsOnly,
};

struct CameraPreset {
    std::string id;
    std::string label;
    math::Vec3 position;
    math::Vec3 focus;
    f32 fov_y_degrees = 35.0f;
};

struct DepthOfFieldSettings {
    bool enabled = false;
    f32 focus_distance = 2.0f;
    f32 aperture = 2.8f;
    f32 bokeh_radius = 0.02f;
};

struct ExposureSettings {
    f32 exposure = 1.0f;
    f32 contrast = 1.0f;
    f32 saturation = 1.0f;
    bool hdr_enabled = true;
};

struct OrbitCameraInput {
    f32 orbit_delta_yaw = 0.0f;
    f32 orbit_delta_pitch = 0.0f;
    f32 zoom_delta = 0.0f;
    math::Vec2 pan_delta{};
    bool set_focus = false;
    math::Vec3 focus_point{};
};

class InspectionCamera {
public:
    void set_mode(CameraMode mode);
    void set_focus(const math::Vec3& focus);
    void set_fov(f32 fov_y_degrees);
    void add_preset(CameraPreset preset);
    void apply_preset(std::string_view preset_id);
    void begin_cinematic_path(std::vector<math::Vec3> path, f32 duration_seconds);
    void focus_body_region(const math::Vec3& center, const math::Vec3& offset = {0.0f, 0.0f, 0.0f});
    void transition_to(const math::Vec3& position, const math::Vec3& focus, f32 duration_seconds);
    void set_content_rating_lockout(ContentRatingLockout lockout);
    void update(f32 delta_seconds, const OrbitCameraInput& input = {});

    [[nodiscard]] math::Mat4 view_matrix() const;
    [[nodiscard]] math::Mat4 projection_matrix(f32 aspect) const;
    [[nodiscard]] math::Vec3 position() const { return position_; }
    [[nodiscard]] math::Vec3 focus() const { return focus_; }
    [[nodiscard]] CameraMode mode() const { return mode_; }
    [[nodiscard]] const DepthOfFieldSettings& depth_of_field() const { return dof_; }
    [[nodiscard]] const ExposureSettings& exposure() const { return exposure_; }

    [[nodiscard]] bool is_locked_out(ContentRatingLockout required_rating) const;

    DepthOfFieldSettings& depth_of_field_mutable() { return dof_; }
    ExposureSettings& exposure_mutable() { return exposure_; }

private:
    void update_orbit(const OrbitCameraInput& input);
    void update_cinematic(f32 delta_seconds);

    CameraMode mode_ = CameraMode::Orbit;
    math::Vec3 position_{0.0f, 1.6f, 3.0f};
    math::Vec3 focus_{0.0f, 1.5f, 0.0f};
    f32 yaw_ = 0.0f;
    f32 pitch_ = 0.15f;
    f32 distance_ = 3.0f;
    f32 fov_y_degrees_ = 35.0f;
    f32 near_z_ = 0.05f;
    f32 far_z_ = 200.0f;
    DepthOfFieldSettings dof_{};
    ExposureSettings exposure_{};
    std::unordered_map<std::string, CameraPreset> presets_;
    std::vector<math::Vec3> cinematic_path_;
    f32 cinematic_duration_ = 0.0f;
    f32 cinematic_elapsed_ = 0.0f;
    math::Vec3 transition_start_pos_{};
    math::Vec3 transition_end_pos_{};
    math::Vec3 transition_start_focus_{};
    math::Vec3 transition_end_focus_{};
    f32 transition_duration_ = 0.0f;
    f32 transition_elapsed_ = 0.0f;
    bool transitioning_ = false;
    ContentRatingLockout rating_lockout_ = ContentRatingLockout::None;
};

} // namespace eve::render
