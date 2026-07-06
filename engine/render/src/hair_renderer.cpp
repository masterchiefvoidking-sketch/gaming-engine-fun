#include <eve/render/hair_renderer.hpp>

namespace eve::render {

HairRenderer::HairRenderer(HumanShaderPipeline& pipeline)
    : pipeline_(pipeline) {}

bool HairRenderer::initialize() {
    return pipeline_.is_ready();
}

void HairRenderer::update(f32 delta_seconds, const math::Vec3& head_position) {
    (void)head_position;
    simulation_time_ += delta_seconds;
    if (settings_.strand_count > settings_.lod0_strands) {
        active_lod_ = 0;
    } else if (settings_.strand_count > settings_.lod1_strands) {
        active_lod_ = 1;
    } else {
        active_lod_ = 2;
    }
}

void HairRenderer::set_settings(const HairRenderSettings& settings) {
    settings_ = settings;
}

u32 HairRenderer::active_strand_count() const {
    switch (active_lod_) {
    case 0:
        return settings_.lod0_strands;
    case 1:
        return settings_.lod1_strands;
    default:
        return settings_.lod2_strands;
    }
}

} // namespace eve::render
