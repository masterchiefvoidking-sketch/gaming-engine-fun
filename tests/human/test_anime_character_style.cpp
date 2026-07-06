#include <eve/human/anime_character_style.hpp>
#include <gtest/gtest.h>

TEST(AnimeCharacterStyleTest, LoadsPresetFromData) {
    eve::human::AnimeCharacterStyleSystem style;
    ASSERT_TRUE(style.load_from_file(EVE_DATA_DIR "/anime/character_preset.json"));
    EXPECT_TRUE(style.apply_style("original_heroine"));
    EXPECT_GT(style.active().body.head_to_body_ratio, 0.2f);
}

TEST(AnimeCharacterStyleTest, SilhouettePresetsAdjustProportions) {
    eve::human::AnimeCharacterStyleSystem style;
    style.apply_silhouette_preset(eve::human::AnimeSilhouettePreset::Petite);
    EXPECT_LT(style.active().body.height, 1.58f);
    style.apply_silhouette_preset(eve::human::AnimeSilhouettePreset::Curvy);
    EXPECT_GT(style.active().body.hip, 0.42f);
}

TEST(AnimeCharacterStyleTest, ComputeTransformScalesByHeight) {
    eve::human::AnimeCharacterStyleSystem style;
    style.apply_silhouette_preset(eve::human::AnimeSilhouettePreset::TallElegant);
    const eve::math::Mat4 transform = style.compute_transform();
    EXPECT_GT(transform.m[0], 0.0f);
}
