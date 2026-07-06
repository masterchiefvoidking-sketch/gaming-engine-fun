#pragma once

#include <eve/core/math/math.hpp>
#include <eve/interaction/input_controller.hpp>
#include <eve/render/inspection_camera.hpp>

#include <string>
#include <unordered_map>

namespace eve::polish {

struct RoomCameraLimits {
    std::string room_id;
    f32 min_distance = 1.2f;
    f32 max_distance = 6.0f;
    f32 min_pitch = -0.3f;
    f32 max_pitch = 1.2f;
    math::Vec3 focus_center{0.0f, 1.5f, 0.0f};
    math::Vec3 bounds_min{-4.0f, 0.0f, -4.0f};
    math::Vec3 bounds_max{4.0f, 3.0f, 4.0f};
};

struct CameraSmoothingSettings {
    f32 acceleration = 8.0f;
    f32 deceleration = 12.0f;
    f32 collision_push_strength = 0.35f;
    f32 privacy_safe_distance = 2.5f;
};

class CameraPolish {
public:
    bool load_room_presets(std::string_view path);
    void set_smoothing(const CameraSmoothingSettings& settings);
    void set_active_room(std::string_view room_id);

    void apply_smooth_input(render::InspectionCamera& camera, f32 delta_seconds,
                            const interaction::InputFrame& input);
    bool apply_room_preset(render::InspectionCamera& camera, std::string_view preset_id);
    bool focus_face(render::InspectionCamera& camera);
    bool focus_full_body(render::InspectionCamera& camera);
    bool focus_mirror(render::InspectionCamera& camera);
    bool focus_sofa_conversation(render::InspectionCamera& camera);
    bool focus_dining_table(render::InspectionCamera& camera);
    bool focus_bedroom_goodnight(render::InspectionCamera& camera);
    bool focus_bathroom_privacy_safe(render::InspectionCamera& camera);
    bool focus_photo_mode(render::InspectionCamera& camera);

    [[nodiscard]] const RoomCameraLimits* active_limits() const;

private:
    math::Vec3 clamp_position(const math::Vec3& pos) const;
    f32 smooth_damp(f32 current, f32 target, f32& velocity, f32 delta) const;

    std::unordered_map<std::string, RoomCameraLimits> room_limits_;
    std::unordered_map<std::string, render::CameraPreset> presets_;
    CameraSmoothingSettings smoothing_{};
    std::string active_room_ = "living_room";
    math::Vec3 velocity_{};
    f32 yaw_velocity_ = 0.0f;
    f32 pitch_velocity_ = 0.0f;
    f32 zoom_velocity_ = 0.0f;
};

} // namespace eve::polish
