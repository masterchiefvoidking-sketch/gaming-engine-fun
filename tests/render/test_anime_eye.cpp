#include <eve/render/anime_eye_system.hpp>
#include <gtest/gtest.h>

TEST(AnimeEyeTest, InitializesWithHighlights) {
    eve::render::AnimeEyeSystem eyes;
    ASSERT_TRUE(eyes.initialize());
    EXPECT_GT(eyes.active_preset().highlights.size(), 0u);
}

TEST(AnimeEyeTest, LoadsPresetsFromData) {
    eve::render::AnimeEyeSystem eyes;
    eyes.initialize();
    eyes.load_presets(EVE_DATA_DIR "/anime/eye_presets.json");
    EXPECT_TRUE(eyes.apply_preset("romance_sparkle"));
}

TEST(AnimeEyeTest, BlinkingUpdatesState) {
    eve::render::AnimeEyeSystem eyes;
    eyes.initialize();
    eyes.trigger_blink();
    eyes.update(0.05f);
    EXPECT_GT(eyes.state().blink_amount, 0.0f);
}

TEST(AnimeEyeTest, GazeModesProduceOffset) {
    eve::render::AnimeEyeSystem eyes;
    eyes.initialize();
    eyes.set_gaze(eve::render::EyeGazeMode::ShyGlance);
    const eve::math::Vec2 offset = eyes.gaze_offset();
    EXPECT_NE(offset.x, 0.0f);
}

TEST(AnimeEyeTest, EyeContactTimerAccumulates) {
    eve::render::AnimeEyeSystem eyes;
    eyes.initialize();
    eyes.set_gaze(eve::render::EyeGazeMode::Direct);
    eyes.update(1.0f);
    EXPECT_GT(eyes.state().eye_contact_timer, 0.0f);
}
