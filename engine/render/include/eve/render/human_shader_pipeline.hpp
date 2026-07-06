#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>
#include <string>

namespace eve::render {

enum class UpscaleTechnology : u8 {
    None = 0,
    DLSS,
    FSR,
    XeSS,
};

struct PBRTextureSet {
    std::string albedo;
    std::string normal;
    std::string roughness;
    std::string metallic;
    std::string ao;
    std::string subsurface;
};

struct SkinShaderParams {
    math::Vec3 albedo_tint{1.0f, 1.0f, 1.0f};
    f32 roughness = 0.45f;
    f32 metallic = 0.0f;
    f32 subsurface_radius = 1.2f;
    f32 subsurface_strength = 0.85f;
    f32 specular = 0.35f;
    f32 micro_normal_strength = 0.5f;
};

struct EyeShaderParams {
    math::Vec3 iris_color{0.25f, 0.45f, 0.65f};
    f32 pupil_size = 0.35f;
    f32 cornea_ior = 1.376f;
    f32 sclera_roughness = 0.25f;
};

struct HairShaderParams {
    math::Vec3 root_color{0.1f, 0.05f, 0.02f};
    math::Vec3 tip_color{0.2f, 0.12f, 0.06f};
    f32 roughness = 0.35f;
    f32 specular_shift = 0.2f;
    bool strand_mode = true;
    u32 strand_count = 12000;
};

struct ClothShaderParams {
    f32 roughness = 0.7f;
    f32 sheen = 0.2f;
    f32 anisotropy = 0.1f;
    f32 wetness = 0.0f;
};

struct MakeupShaderParams {
    f32 lipstick_intensity = 0.0f;
    f32 blush_intensity = 0.0f;
    f32 eyeshadow_intensity = 0.0f;
    math::Vec3 lipstick_color{0.8f, 0.15f, 0.25f};
};

struct JewelryShaderParams {
    f32 metallic = 0.95f;
    f32 roughness = 0.15f;
    f32 gem_refraction = 1.77f;
};

struct HumanMaterialProfile {
    SkinShaderParams skin;
    EyeShaderParams eyes;
    HairShaderParams hair;
    ClothShaderParams cloth;
    MakeupShaderParams makeup;
    JewelryShaderParams jewelry;
    PBRTextureSet textures;
};

class HumanShaderPipeline {
public:
    bool initialize();
    void shutdown();

    [[nodiscard]] bool is_ready() const { return ready_; }
    [[nodiscard]] HumanMaterialProfile& material_mutable() { return material_; }
    [[nodiscard]] const HumanMaterialProfile& material() const { return material_; }
    [[nodiscard]] UpscaleTechnology upscale_technology() const { return upscale_; }

    void apply_skin_params(const SkinShaderParams& skin);
    void apply_hair_params(const HairShaderParams& hair);
    void set_cloth_wetness(f32 wetness);
    void set_makeup_params(const MakeupShaderParams& makeup);
    void set_jewelry_params(const JewelryShaderParams& jewelry);
    void set_upscale_technology(UpscaleTechnology tech) { upscale_ = tech; }
    void enable_ssr(bool enabled) { ssr_enabled_ = enabled; }
    void enable_ssao(bool enabled) { ssao_enabled_ = enabled; }
    void enable_bloom(bool enabled) { bloom_enabled_ = enabled; }
    void enable_volumetrics(bool enabled) { volumetrics_enabled_ = enabled; }

private:
    bool ready_ = false;
    HumanMaterialProfile material_{};
    UpscaleTechnology upscale_ = UpscaleTechnology::None;
    bool ssr_enabled_ = true;
    bool ssao_enabled_ = true;
    bool bloom_enabled_ = true;
    bool volumetrics_enabled_ = true;
};

} // namespace eve::render
