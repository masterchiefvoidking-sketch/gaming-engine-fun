#include <eve/polish/graphics_polish.hpp>

#include <algorithm>

namespace eve::polish {

void GraphicsPolish::apply_preset(GraphicsPreset preset) {
    settings_.preset = preset;
    switch (preset) {
    case GraphicsPreset::Low:
        settings_.outline_thickness = 0.9f;
        settings_.toon_ramp_softness = 0.2f;
        settings_.skin_warmth = 0.06f;
        settings_.eye_highlight_intensity = 0.8f;
        settings_.blush_blend = 0.35f;
        settings_.hair_highlight = 0.2f;
        settings_.shadow_softness = 0.4f;
        settings_.dof_strength = 0.0f;
        settings_.color_grade_warmth = 0.04f;
        settings_.screenshot_quality = 0.75f;
        settings_.mirror_reflections = false;
        break;
    case GraphicsPreset::Medium:
        settings_.outline_thickness = 1.0f;
        settings_.toon_ramp_softness = 0.28f;
        settings_.skin_warmth = 0.1f;
        settings_.eye_highlight_intensity = 1.0f;
        settings_.blush_blend = 0.45f;
        settings_.hair_highlight = 0.3f;
        settings_.shadow_softness = 0.55f;
        settings_.dof_strength = 0.25f;
        settings_.color_grade_warmth = 0.06f;
        settings_.screenshot_quality = 0.9f;
        break;
    case GraphicsPreset::High:
        settings_.outline_thickness = 1.15f;
        settings_.toon_ramp_softness = 0.35f;
        settings_.skin_warmth = 0.12f;
        settings_.eye_highlight_intensity = 1.2f;
        settings_.blush_blend = 0.55f;
        settings_.hair_highlight = 0.4f;
        settings_.shadow_softness = 0.65f;
        settings_.dof_strength = 0.5f;
        settings_.color_grade_warmth = 0.08f;
        settings_.screenshot_quality = 1.0f;
        break;
    case GraphicsPreset::Ultra:
        settings_.outline_thickness = 1.25f;
        settings_.toon_ramp_softness = 0.42f;
        settings_.skin_warmth = 0.14f;
        settings_.eye_highlight_intensity = 1.4f;
        settings_.blush_blend = 0.62f;
        settings_.hair_highlight = 0.5f;
        settings_.shadow_softness = 0.75f;
        settings_.dof_strength = 0.7f;
        settings_.color_grade_warmth = 0.1f;
        settings_.screenshot_quality = 1.0f;
        break;
    case GraphicsPreset::WebSafe:
        settings_.outline_thickness = 1.05f;
        settings_.toon_ramp_softness = 0.3f;
        settings_.skin_warmth = 0.1f;
        settings_.eye_highlight_intensity = 1.0f;
        settings_.blush_blend = 0.5f;
        settings_.hair_highlight = 0.28f;
        settings_.shadow_softness = 0.5f;
        settings_.dof_strength = 0.2f;
        settings_.color_grade_warmth = 0.06f;
        settings_.screenshot_quality = 0.85f;
        settings_.mirror_reflections = true;
        break;
    case GraphicsPreset::MobileFuture:
        settings_.outline_thickness = 0.85f;
        settings_.toon_ramp_softness = 0.22f;
        settings_.skin_warmth = 0.08f;
        settings_.eye_highlight_intensity = 0.9f;
        settings_.blush_blend = 0.4f;
        settings_.hair_highlight = 0.22f;
        settings_.shadow_softness = 0.45f;
        settings_.dof_strength = 0.0f;
        settings_.color_grade_warmth = 0.05f;
        settings_.screenshot_quality = 0.7f;
        settings_.mirror_reflections = false;
        break;
    }
}

void GraphicsPolish::apply_to_material(render::AnimeHumanMaterial& material) const {
    render::AnimeSkinParams skin = material.skin();
    skin.blush_intensity = settings_.blush_blend;
    skin.rim_light_intensity = settings_.hair_highlight * 0.5f;
    skin.base_tone.x += settings_.skin_warmth * 0.05f;
    skin.base_tone.y += settings_.skin_warmth * 0.02f;
    skin.soft_shadow_band = settings_.toon_ramp_softness;
    material.set_skin_params(skin);
    render::AnimeOutlineParams outline = material.outline();
    outline.thickness = settings_.outline_thickness;
    material.set_outline_params(outline);
}

void GraphicsPolish::apply_to_outline(render::AnimeOutlinePass& outline) const {
    render::OutlinePassSettings pass;
    pass.enabled = true;
    pass.thickness = settings_.outline_thickness;
    pass.color = {0.1f, 0.07f, 0.12f};
    pass.depth_bias = 0.0015f;
    outline.set_settings(pass);
}

void GraphicsPolish::apply_to_pipeline(render::HumanShaderPipeline& pipeline) const {
    pipeline.enable_bloom(settings_.dof_strength > 0.1f);
    pipeline.enable_ssao(settings_.shadow_softness > 0.5f);
    pipeline.set_cloth_wetness(settings_.shadow_softness * 0.1f);
}

void GraphicsPolish::apply_to_photo_mode(render::PhotoMode& photo) const {
    render::PhotoModeSettings settings;
    settings.color_grading = true;
    settings.background_blur = settings_.dof_strength > 0.0f;
    settings.lighting_preset = settings_.warm_apartment_lighting ? "warm_apartment" : "studio_soft";
    settings.screenshot_scale = settings_.screenshot_quality >= 1.0f ? 2u : 1u;
    photo.set_settings(settings);
}

void GraphicsPolish::apply_quality_tier(interaction::QualitySettings& quality) const {
    switch (settings_.preset) {
    case GraphicsPreset::Low:
        quality.apply_tier(interaction::QualityTier::Low);
        break;
    case GraphicsPreset::Medium:
    case GraphicsPreset::WebSafe:
    case GraphicsPreset::MobileFuture:
        quality.apply_tier(interaction::QualityTier::Medium);
        break;
    case GraphicsPreset::High:
        quality.apply_tier(interaction::QualityTier::High);
        break;
    case GraphicsPreset::Ultra:
        quality.apply_tier(interaction::QualityTier::Ultra);
        break;
    }
    if (settings_.preset == GraphicsPreset::WebSafe) {
        quality.frame_cap = 60;
        quality.resolution_scale = std::min(quality.resolution_scale, 0.9f);
    }
    if (settings_.preset == GraphicsPreset::MobileFuture) {
        quality.apply_battery_saver();
    }
}

} // namespace eve::polish
