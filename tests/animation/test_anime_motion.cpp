#include <eve/animation/anime_motion.hpp>
#include <gtest/gtest.h>

TEST(AnimeMotionTest, ShyBodyLanguageSetsHeadTilt) {
    eve::animation::AnimeMotionController motion;
    motion.set_body_language(eve::animation::AnimeBodyLanguageId::Shy);
    motion.update(1.0f / 60.0f);
    EXPECT_LT(motion.state().head_tilt, 0.0f);
}

TEST(AnimeMotionTest, BreathingOscillates) {
    eve::animation::AnimeMotionController motion;
  motion.update(0.5f);
    EXPECT_NE(motion.state().breathing_offset, 0.0f);
}

TEST(AnimeMotionTest, ReactionTriggersEmbarrassed) {
    eve::animation::AnimeMotionController motion;
    motion.trigger_reaction("embarrassed");
    EXPECT_EQ(motion.state().active_clip, "anim_embarrassed");
}

TEST(AnimeMotionTest, OutfitPreviewWalkClip) {
    eve::animation::AnimeMotionController motion;
    motion.set_body_language(eve::animation::AnimeBodyLanguageId::OutfitPreviewWalk);
    EXPECT_EQ(motion.state().active_clip, "anim_walk_preview");
}
