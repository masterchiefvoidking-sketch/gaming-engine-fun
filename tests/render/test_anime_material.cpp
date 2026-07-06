#include <eve/render/anime_human_material.hpp>
#include <gtest/gtest.h>

TEST(AnimeMaterialTest, InitializesWithToonRamp) {
    eve::render::AnimeHumanMaterial material;
    ASSERT_TRUE(material.initialize());
    EXPECT_GT(material.toon_ramp().size(), 0u);
    material.shutdown();
}

TEST(AnimeMaterialTest, LoadsPresetsFromData) {
    eve::render::AnimeHumanMaterial material;
    material.initialize();
    material.load_presets(EVE_DATA_DIR "/anime/materials.json");
  EXPECT_TRUE(material.apply_preset("warm_romance"));
    EXPECT_GT(material.skin().blush_intensity, 0.0f);
}

TEST(AnimeMaterialTest, BlushAndMoodTint) {
    eve::render::AnimeHumanMaterial material;
    material.initialize();
    material.set_blush(0.6f);
    material.set_mood_tint({1.0f, 0.9f, 0.85f});
    EXPECT_NEAR(material.skin().blush_intensity, 0.6f, 0.01f);
}

TEST(AnimeMaterialTest, ToonRampEvaluation) {
    const std::vector<eve::render::ToonRampStop> ramp = {
        {0.0f, {0.2f, 0.2f, 0.2f}}, {1.0f, {1.0f, 1.0f, 1.0f}}};
    const eve::f32 low = eve::render::evaluate_toon_ramp(0.1f, ramp);
    const eve::f32 high = eve::render::evaluate_toon_ramp(0.9f, ramp);
    EXPECT_LT(low, high);
}
