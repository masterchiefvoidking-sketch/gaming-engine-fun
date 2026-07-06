#pragma once

#include <eve/render/inspection_camera.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::render {

enum class RomanceCameraPresetId : u8 {
    FullBodyWardrobe = 0,
    FaceCloseUp,
    EyeCloseUp,
    OverTheShoulder,
    SittingConversation,
    SofaConversation,
    BedsideConversation,
    DiningTable,
    MirrorView,
    BalconyNight,
    WalkingFollow,
    PhotoPortrait,
    Count
};

struct RomanceCameraPreset {
    std::string id;
    std::string label;
    math::Vec3 position;
    math::Vec3 focus;
    f32 fov_y_degrees = 35.0f;
    f32 focal_length_mm = 50.0f;
    DepthOfFieldSettings depth_of_field;
    bool rule_of_thirds_guide = false;
};

struct RuleOfThirdsGuide {
    bool enabled = false;
    math::Vec2 intersection_point{0.0f, 0.0f};
};

class RomanceCameraController {
public:
    void attach(InspectionCamera& camera);
    void load_presets(std::string_view path);
    void register_preset(RomanceCameraPreset preset);
    bool apply_preset(std::string_view preset_id);
    bool apply_romance_preset(RomanceCameraPresetId preset_id);
    void enable_rule_of_thirds(bool enabled);
    void set_focal_length(f32 focal_length_mm);
    void transition_to_preset(std::string_view preset_id, f32 duration_seconds);

    [[nodiscard]] const RuleOfThirdsGuide& composition_guide() const { return guide_; }
    [[nodiscard]] const RomanceCameraPreset* active_preset() const { return active_preset_; }

private:
    InspectionCamera* camera_ = nullptr;
    RuleOfThirdsGuide guide_{};
    std::unordered_map<std::string, RomanceCameraPreset> presets_;
    const RomanceCameraPreset* active_preset_ = nullptr;
};

} // namespace eve::render
