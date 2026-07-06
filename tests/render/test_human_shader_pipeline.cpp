#include <eve/human/character_profile.hpp>
#include <eve/human/render_sync.hpp>
#include <eve/render/human_shader_pipeline.hpp>
#include <gtest/gtest.h>

TEST(HumanShaderPipelineTest, SyncsCharacterProfile) {
    eve::render::HumanShaderPipeline pipeline;
    ASSERT_TRUE(pipeline.initialize());
    eve::human::CharacterProfile profile;
    profile.skin.skin_tone = {0.95f, 0.8f, 0.7f};
    profile.skin.subsurface_strength = 0.9f;
    eve::human::sync_character_to_pipeline(profile, pipeline);
    EXPECT_NEAR(pipeline.material().skin.albedo_tint.x, 0.95f, 0.01f);
    pipeline.shutdown();
}
