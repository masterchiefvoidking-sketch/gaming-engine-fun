#pragma once

#include <eve/interaction/quality_settings.hpp>
#include <eve/platform/mobile_input.hpp>

namespace eve::interaction {

struct WebPerformanceProfile {
    bool webgpu_renderer = true;
    bool wasm_build = true;
    bool compress_assets = true;
    bool progressive_loading = true;
    bool texture_streaming = true;
    u32 shader_quality_tier = 2;
    u32 animation_lod = 1;
    u32 character_lod = 1;
    u32 hair_quality = 2;
    u32 reflection_quality = 1;
    bool mobile_fallback = true;
    QualitySettings quality;
    platform::MobileRenderSettings mobile;

    void apply_web_defaults();
    void apply_mobile_defaults();
    void sync_from_quality();
};

} // namespace eve::interaction
