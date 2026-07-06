#include <eve/human/character_framework.hpp>
#include <gtest/gtest.h>

TEST(CharacterFrameworkTest, InitializesSubsystems) {
    eve::human::CharacterFramework framework;
    ASSERT_TRUE(framework.initialize());
    EXPECT_TRUE(framework.shader_pipeline().is_ready());
    framework.update(1.0f / 60.0f);
    EXPECT_EQ(framework.animation().current_state(), eve::animation::AnimationStateId::Idle);
    framework.shutdown();
}
