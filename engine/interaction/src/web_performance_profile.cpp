#include <eve/interaction/web_performance_profile.hpp>

namespace eve::interaction {

void WebPerformanceProfile::apply_web_defaults() {
    webgpu_renderer = true;
    wasm_build = true;
    compress_assets = true;
    progressive_loading = true;
    texture_streaming = true;
    shader_quality_tier = 2;
    animation_lod = 1;
    character_lod = 1;
    hair_quality = 2;
    reflection_quality = 1;
    mobile_fallback = true;
    quality.apply_tier(QualityTier::High);
    quality.frame_cap = 60;
    sync_from_quality();
}

void WebPerformanceProfile::apply_mobile_defaults() {
    apply_web_defaults();
    quality.apply_tier(QualityTier::Medium);
    quality.apply_battery_saver();
    mobile.quality_level = 1;
    mobile.reduce_hair_physics = true;
    mobile.limit_shadows = true;
    mobile.battery_saver = true;
    shader_quality_tier = 1;
    animation_lod = 2;
    character_lod = 2;
    hair_quality = 1;
    reflection_quality = 0;
    sync_from_quality();
}

void WebPerformanceProfile::sync_from_quality() {
    mobile.quality_level = static_cast<u32>(quality.texture_quality);
    mobile.reduce_hair_physics = quality.hair_quality <= QualityTier::Low;
    mobile.limit_shadows = quality.shadow_quality <= QualityTier::Medium;
    mobile.target_frame_time_ms = quality.frame_cap > 0 ? 1000.0f / static_cast<f32>(quality.frame_cap)
                                                        : 16.67f;
    mobile.battery_saver = quality.battery_saver;
}

} // namespace eve::interaction
