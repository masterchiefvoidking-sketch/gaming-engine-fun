#pragma once

#include <eve/render/human_shader_pipeline.hpp>
#include <eve/render/inspection_camera.hpp>
#include <string>
#include <unordered_map>

namespace eve::render {

struct PhotoModeLightingPreset {
    std::string id;
    std::string label;
    math::Vec3 key_light_direction{0.3f, -0.8f, 0.4f};
    f32 key_intensity = 2.5f;
    f32 fill_intensity = 0.6f;
    f32 rim_intensity = 1.2f;
};

struct PhotoModeSettings {
    u32 screenshot_scale = 2;
    bool background_blur = true;
    bool color_grading = true;
    f32 time_of_day = 0.35f;
    std::string expression_preset = "neutral";
    std::string lighting_preset = "studio_soft";
};

class PhotoMode {
public:
    PhotoMode(InspectionCamera& camera, HumanShaderPipeline& pipeline);

    void enable(bool enabled);
    void set_settings(const PhotoModeSettings& settings);
    void add_lighting_preset(PhotoModeLightingPreset preset);
    void capture_screenshot(std::string_view output_path) const;

    [[nodiscard]] bool is_enabled() const { return enabled_; }
    [[nodiscard]] const PhotoModeSettings& settings() const { return settings_; }
    [[nodiscard]] const InspectionCamera& camera() const { return camera_; }

private:
    InspectionCamera& camera_;
    HumanShaderPipeline& pipeline_;
    bool enabled_ = false;
    PhotoModeSettings settings_{};
    std::unordered_map<std::string, PhotoModeLightingPreset> lighting_presets_;
};

} // namespace eve::render
