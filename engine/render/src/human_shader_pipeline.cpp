#include <algorithm>
#include <eve/core/logging/logger.hpp>
#include <eve/render/human_shader_pipeline.hpp>

namespace eve::render {

bool HumanShaderPipeline::initialize() {
    material_.textures.albedo = "textures/skin/albedo.png";
    material_.textures.normal = "textures/skin/normal.png";
    material_.textures.roughness = "textures/skin/roughness.png";
    material_.textures.subsurface = "textures/skin/sss.png";
    ready_ = true;
    EVE_LOG(Info, "Render", "Human PBR shader pipeline initialized");
    return true;
}

void HumanShaderPipeline::shutdown() {
    ready_ = false;
}

void HumanShaderPipeline::apply_skin_params(const SkinShaderParams& skin) {
    material_.skin = skin;
}

void HumanShaderPipeline::apply_hair_params(const HairShaderParams& hair) {
    material_.hair = hair;
}

void HumanShaderPipeline::set_cloth_wetness(f32 wetness) {
    material_.cloth.wetness = std::clamp(wetness, 0.0f, 1.0f);
}

void HumanShaderPipeline::set_makeup_params(const MakeupShaderParams& makeup) {
    material_.makeup = makeup;
}

void HumanShaderPipeline::set_jewelry_params(const JewelryShaderParams& jewelry) {
    material_.jewelry = jewelry;
}

} // namespace eve::render
