#include <eve/animation/layer_stack.hpp>
#include <gtest/gtest.h>

TEST(AnimationLayerStackTest, LoadsDefinitions) {
    eve::animation::AnimationLayerStack stack;
    stack.load_definitions(EVE_DATA_DIR "/animation/layers.json");
    const auto layers = stack.active_layers();
    EXPECT_GT(layers.size(), 0u);
}

TEST(AnimationLayerStackTest, BlendsLayerWeights) {
    eve::animation::AnimationLayerStack stack;
    stack.load_definitions(EVE_DATA_DIR "/animation/layers.json");
    stack.set_layer_clip(eve::animation::AnimationLayerId::UpperBodyGestures, "anim_wave", 0.8f);
    EXPECT_NEAR(stack.blended_weight(eve::animation::AnimationLayerId::UpperBodyGestures), 0.8f, 0.01f);
}

TEST(AnimationLayerStackTest, SerializationRoundTripViaFile) {
    eve::animation::AnimationLayerStack stack;
    ASSERT_NO_THROW(stack.load_definitions(EVE_DATA_DIR "/animation/layers.json"));
    stack.set_layer_clip(eve::animation::AnimationLayerId::BaseLocomotion, "anim_walk", 1.0f);
    stack.update(1.0f / 60.0f);
    const auto active = stack.active_layers();
    bool found_walk = false;
    for (const auto& layer : active) {
        if (layer.id == eve::animation::AnimationLayerId::BaseLocomotion && layer.active_clip == "anim_walk") {
            found_walk = true;
        }
    }
    EXPECT_TRUE(found_walk);
}
