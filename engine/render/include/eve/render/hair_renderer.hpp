#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>
#include <eve/render/human_shader_pipeline.hpp>

namespace eve::render {

enum class HairRenderMode : u8 {
    Strand = 0,
    Card,
};

struct HairRenderSettings {
    HairRenderMode mode = HairRenderMode::Strand;
    u32 strand_count = 12000;
    u32 lod0_strands = 12000;
    u32 lod1_strands = 6000;
    u32 lod2_strands = 2000;
    f32 wind_strength = 0.2f;
    math::Vec3 wind_direction{1.0f, 0.0f, 0.2f};
    bool collision_enabled = true;
};

class HairRenderer {
public:
    explicit HairRenderer(HumanShaderPipeline& pipeline);

    bool initialize();
    void update(f32 delta_seconds, const math::Vec3& head_position);
    void set_settings(const HairRenderSettings& settings);

    [[nodiscard]] const HairRenderSettings& settings() const { return settings_; }
    [[nodiscard]] u32 active_strand_count() const;
    [[nodiscard]] u32 active_lod() const { return active_lod_; }

private:
    HumanShaderPipeline& pipeline_;
    HairRenderSettings settings_{};
    u32 active_lod_ = 0;
    f32 simulation_time_ = 0.0f;
};

} // namespace eve::render
