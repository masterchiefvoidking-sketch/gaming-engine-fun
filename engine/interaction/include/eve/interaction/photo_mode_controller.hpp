#pragma once

#include <eve/human/anime_visual_pipeline.hpp>
#include <eve/render/inspection_camera.hpp>

#include <string>

namespace eve::interaction {

struct PhotoControllerSettings {
    u32 screenshot_scale = 2;
    bool background_blur = true;
    std::string expression_preset = "neutral";
    std::string lighting_preset = "studio_soft";
};

class PhotoModeController {
public:
    bool initialize(render::InspectionCamera& camera, human::AnimeVisualPipeline& pipeline);
    void enable(bool enabled);
    void set_lighting_preset(std::string_view preset_id);
    void set_expression(std::string_view expression_id);
    void set_outfit(std::string_view outfit_id);
    void set_hair(std::string_view hair_id);
    void set_background_blur(bool enabled);
    void set_hide_ui(bool hidden);
    bool capture(std::string_view path);

    [[nodiscard]] bool is_enabled() const { return enabled_; }
    [[nodiscard]] bool hide_ui() const { return hide_ui_; }
    [[nodiscard]] const PhotoControllerSettings& settings() const { return settings_; }

private:
    render::InspectionCamera* camera_ = nullptr;
    human::AnimeVisualPipeline* pipeline_ = nullptr;
    PhotoControllerSettings settings_{};
    bool enabled_ = false;
    bool hide_ui_ = false;
};

} // namespace eve::interaction
