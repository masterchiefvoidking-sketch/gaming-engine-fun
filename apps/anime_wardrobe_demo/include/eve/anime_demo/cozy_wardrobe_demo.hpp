#pragma once

#include <eve/human/anime_visual_pipeline.hpp>

namespace eve::anime_demo {

class CozyWardrobeDemo {
public:
    bool initialize(std::string_view data_root);
    void shutdown();
    void update(f32 delta_seconds);

    void orbit(f32 yaw, f32 pitch, f32 zoom);
    void pan(f32 x, f32 y);
    bool switch_outfit(std::string_view item_id);
    bool switch_hair(std::string_view style_id);
    bool switch_expression(std::string_view expression_id);
    void toggle_blush();
    void apply_face_closeup();
    void apply_full_body();
    bool take_screenshot(std::string_view path);

    [[nodiscard]] human::AnimeVisualPipeline& pipeline() { return pipeline_; }

private:
    human::AnimeVisualPipeline pipeline_;
    bool blush_on_ = true;
};

} // namespace eve::anime_demo
