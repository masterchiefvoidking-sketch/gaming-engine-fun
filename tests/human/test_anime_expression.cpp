#include <eve/human/anime_expression_system.hpp>
#include <gtest/gtest.h>

TEST(AnimeExpressionTest, LoadsAllPresets) {
    eve::human::AnimeExpressionSystem expressions;
    ASSERT_TRUE(expressions.load_presets(EVE_DATA_DIR "/anime/expressions.json"));
    EXPECT_NE(expressions.find("romantic_gaze"), nullptr);
}

TEST(AnimeExpressionTest, BlendsToTargetExpression) {
    eve::human::AnimeExpressionSystem expressions;
    expressions.load_presets(EVE_DATA_DIR "/anime/expressions.json");
    expressions.set_expression("blushing");
    for (int i = 0; i < 30; ++i) {
        expressions.update(1.0f / 60.0f);
    }
    EXPECT_GT(expressions.active_channels().blush, 0.5f);
    EXPECT_TRUE(expressions.blush_enabled());
}

TEST(AnimeExpressionTest, ShyExpressionTiltsHead) {
    eve::human::AnimeExpressionSystem expressions;
    expressions.load_presets(EVE_DATA_DIR "/anime/expressions.json");
    expressions.set_expression("shy");
    for (int i = 0; i < 30; ++i) {
        expressions.update(1.0f / 60.0f);
    }
    EXPECT_LT(expressions.active_channels().head_tilt, 0.0f);
}
