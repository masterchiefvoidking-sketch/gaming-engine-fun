#include <eve/human/character_profile.hpp>
#include <filesystem>
#include <gtest/gtest.h>

TEST(CharacterCreatorTest, AppliesPresetsAndMorphs) {
    eve::human::CharacterCreator creator;
    creator.apply_body_preset(eve::human::BodyTypePreset::Curvy);
    EXPECT_GT(creator.profile().body.bust, 0.6f);
    creator.set_morph("smile", 0.7f);
    EXPECT_NEAR(creator.morph("smile"), 0.7f, 0.01f);
}

TEST(CharacterCreatorTest, SavesAndLoadsProfile) {
    eve::human::CharacterCreator creator;
    creator.apply_body_preset(eve::human::BodyTypePreset::Athletic);
    const std::string path = "test_character_profile.json";
    ASSERT_TRUE(creator.save_to_file(path));

    eve::human::CharacterCreator loaded;
    ASSERT_TRUE(loaded.load_from_file(path));
    EXPECT_NEAR(loaded.profile().body.muscle_definition, 0.7f, 0.01f);
    std::filesystem::remove(path);
}

TEST(CharacterCreatorTest, ComputesBodyTransform) {
    eve::human::CharacterCreator creator;
    creator.set_profile(creator.profile());
    eve::human::CharacterProfile profile = creator.profile();
    profile.body.height = 1.8f;
    creator.set_profile(std::move(profile));
    const eve::math::Mat4 transform = creator.compute_body_transform();
    EXPECT_GT(transform.m[5], 1.0f);
}
