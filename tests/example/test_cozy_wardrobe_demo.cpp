#include <eve/anime_demo/cozy_wardrobe_demo.hpp>
#include <gtest/gtest.h>

TEST(CozyWardrobeDemoTest, RunsFullDemoFlow) {
    eve::anime_demo::CozyWardrobeDemo demo;
    ASSERT_TRUE(demo.initialize(EVE_DATA_DIR));

    demo.orbit(0.1f, 0.0f, 0.0f);
    demo.apply_full_body();
    demo.switch_outfit("date_dress");
    demo.switch_hair("twin_tails");
    demo.switch_expression("romantic_gaze");
    demo.apply_face_closeup();
    demo.toggle_blush();

    for (int i = 0; i < 60; ++i) {
        demo.update(1.0f / 60.0f);
    }

    EXPECT_TRUE(demo.take_screenshot("test_anime_screenshot.hdr"));
    demo.shutdown();
}
