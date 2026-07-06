#include <eve/example/apartment_character_scene.hpp>
#include <gtest/gtest.h>

TEST(ApartmentCharacterSceneTest, LoadsAndInteracts) {
    eve::example::ApartmentCharacterScene scene;
    ASSERT_TRUE(scene.initialize(EVE_DATA_DIR));
    ASSERT_TRUE(scene.navigate_to_room("living_room"));
    EXPECT_TRUE(scene.interact_with("sofa", eve::human::CharacterInteractionType::Sit));
    scene.update(1.0f / 60.0f);
    scene.shutdown();
}
