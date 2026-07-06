#include <eve/human/anime_visual_pipeline.hpp>
#include <gtest/gtest.h>

TEST(AnimeVisualPipelineTest, InitializesFullStack) {
    eve::human::AnimeVisualPipeline pipeline;
    ASSERT_TRUE(pipeline.initialize(EVE_DATA_DIR));
    EXPECT_TRUE(pipeline.material().is_ready());
    pipeline.shutdown();
}

TEST(AnimeVisualPipelineTest, SwitchesOutfitHairExpression) {
    eve::human::AnimeVisualPipeline pipeline;
    ASSERT_TRUE(pipeline.initialize(EVE_DATA_DIR));
    EXPECT_TRUE(pipeline.change_outfit("date_dress"));
    EXPECT_TRUE(pipeline.change_hair("wavy"));
    EXPECT_TRUE(pipeline.set_expression("flirty"));
    pipeline.shutdown();
}

TEST(AnimeVisualPipelineTest, BlushToggle) {
    eve::human::AnimeVisualPipeline pipeline;
    pipeline.initialize(EVE_DATA_DIR);
    pipeline.set_blush_enabled(false);
    EXPECT_NEAR(pipeline.material().skin().blush_intensity, 0.0f, 0.01f);
    pipeline.shutdown();
}
