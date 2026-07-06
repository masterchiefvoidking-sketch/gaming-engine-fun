#include <eve/editor/creator_studio.hpp>

#include <gtest/gtest.h>

#include <filesystem>

TEST(CreatorStudioTest, OpensApartmentLifeDemo) {
    eve::editor::CreatorStudio studio;
    eve::editor::CreatorStudioConfig config;
    config.project_root = EVE_GAME_DIR "/ApartmentLifeDemo";
    config.title = "Apartment Life Demo";
    ASSERT_TRUE(studio.initialize(config));
    ASSERT_TRUE(studio.open_project(EVE_GAME_DIR "/ApartmentLifeDemo"));

    EXPECT_EQ(studio.project().characters().character_count(), 1u);
    EXPECT_GE(studio.project().wardrobe().item_count(), 20u);
    EXPECT_GE(studio.project().dialogue().package_count(), 20u);

    const auto results = studio.asset_browser().search("mira");
    EXPECT_FALSE(results.empty());
}

TEST(CreatorStudioTest, CharacterEditorCreatesEntry) {
    eve::editor::CreatorStudio studio;
    studio.initialize({});
    studio.character_editor().set_project(studio.project());
    EXPECT_TRUE(studio.character_editor().create_character("test_npc", "Test NPC"));
}

TEST(CreatorStudioTest, PhotoStudioCaptures) {
    eve::editor::PhotoStudioPanel photo;
    eve::editor::PhotoStudioSettings settings;
    settings.pose_id = "mirror_pose";
    settings.expression_id = "soft_smile";
    photo.set_settings(settings);
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / "eve_photo_test.json";
    EXPECT_TRUE(photo.capture_screenshot(path.string()));
    std::filesystem::remove(path);
}

TEST(CreatorStudioTest, ExportsWindowsBuild) {
    eve::editor::CreatorStudio studio;
    eve::editor::CreatorStudioConfig config;
    config.title = "Apartment Life Demo";
    ASSERT_TRUE(studio.initialize(config));
    ASSERT_TRUE(studio.open_project(EVE_GAME_DIR "/ApartmentLifeDemo"));
    EXPECT_TRUE(studio.export_game(eve::content::ExportTarget::Windows));
}
