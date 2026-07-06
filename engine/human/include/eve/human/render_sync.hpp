#pragma once

#include <eve/human/character_profile.hpp>
#include <eve/render/human_shader_pipeline.hpp>

namespace eve::human {

inline void sync_character_to_pipeline(const CharacterProfile& profile,
                                       render::HumanShaderPipeline& pipeline) {
    render::SkinShaderParams skin;
    skin.albedo_tint = profile.skin.skin_tone;
    skin.subsurface_strength = profile.skin.subsurface_strength;
    pipeline.apply_skin_params(skin);

    render::HairShaderParams hair;
    hair.root_color = profile.hair.root_color;
    hair.tip_color = profile.hair.highlight_color;
    hair.strand_mode = profile.hair.physics_enabled;
    pipeline.apply_hair_params(hair);
}

} // namespace eve::human
