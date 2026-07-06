#pragma once

#include <eve/interaction/quality_settings.hpp>
#include <eve/render/anime_human_material.hpp>
#include <eve/render/anime_outline_pass.hpp>
#include <eve/render/human_shader_pipeline.hpp>
#include <eve/render/photo_mode.hpp>

namespace eve::polish {

enum class GraphicsPreset : u8 {
    Low = 0,
    Medium,
    High,
    Ultra,
    WebSafe,
    MobileFuture,
};

struct GraphicsPolishSettings {
    GraphicsPreset preset = GraphicsPreset::High;
    f32 outline_thickness = 1.15f;
    f32 toon_ramp_softness = 0.35f;
    f32 skin_warmth = 0.12f;
    f32 eye_highlight_intensity = 1.2f;
    f32 blush_blend = 0.55f;
    f32 hair_highlight = 0.4f;
    f32 shadow_softness = 0.65f;
    f32 dof_strength = 0.5f;
    f32 color_grade_warmth = 0.08f;
    f32 screenshot_quality = 1.0f;
    bool mirror_reflections = true;
    bool warm_apartment_lighting = true;
};

class GraphicsPolish {
public:
    void apply_preset(GraphicsPreset preset);
    void apply_to_material(render::AnimeHumanMaterial& material) const;
    void apply_to_outline(render::AnimeOutlinePass& outline) const;
    void apply_to_pipeline(render::HumanShaderPipeline& pipeline) const;
    void apply_to_photo_mode(render::PhotoMode& photo) const;
    void apply_quality_tier(interaction::QualitySettings& quality) const;

    [[nodiscard]] const GraphicsPolishSettings& settings() const { return settings_; }
    GraphicsPolishSettings& settings_mutable() { return settings_; }

private:
    GraphicsPolishSettings settings_{};
};

} // namespace eve::polish
