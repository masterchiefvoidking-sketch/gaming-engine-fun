#include <eve/content/asset_import.hpp>
#include <eve/content/content_database.hpp>
#include <eve/content/game_content_project.hpp>
#include <eve/content/localization.hpp>
#include <eve/content/packaging.hpp>
#include <eve/content/undo_stack.hpp>
#include <eve/content/wardrobe_database.hpp>

#include <eve/core/filesystem/filesystem.hpp>
#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";

} // namespace

TEST(ContentDatabaseTest, RegistersAndSearchesAssets) {
    eve::content::ContentDatabase db;
    eve::content::ContentAsset asset;
    asset.type = eve::content::AssetType::Character;
    asset.metadata.id = "test_char";
    asset.metadata.display_name = "Test Character";
    asset.metadata.tags = {"hero", "original"};
    ASSERT_TRUE(db.register_asset(std::move(asset)));
    EXPECT_EQ(db.asset_count(), 1u);

    const auto results = db.search("test");
    ASSERT_EQ(results.size(), 1u);
    EXPECT_EQ(results[0].asset_id, "test_char");
}

TEST(ProjectLayoutTest, CreatesStandardFolders) {
    const std::filesystem::path temp =
        std::filesystem::temp_directory_path() / "eve_test_project";
    eve::content::ProjectLayout layout;
    layout.initialize(temp.string());
    layout.set_title("Test");
    EXPECT_TRUE(layout.create_default_structure());
    EXPECT_TRUE(eve::FileSystem::is_directory((temp / "Assets" / "Dialogue").string()));
    std::filesystem::remove_all(temp);
}

TEST(UndoStackTest, UndoRedoRoundTrip) {
    eve::content::UndoStack stack;
    int value = 0;
    stack.push({"increment", [&]() { --value; }, [&]() { ++value; }});
    value = 1;
    EXPECT_TRUE(stack.can_undo());
    EXPECT_TRUE(stack.undo());
    EXPECT_EQ(value, 0);
    EXPECT_TRUE(stack.redo());
    EXPECT_EQ(value, 1);
}

TEST(WardrobeDatabaseTest, LoadsClothingItem) {
    eve::content::WardrobeDatabase wardrobe;
    ASSERT_TRUE(wardrobe.load_item(std::string(kGameRoot) + "/Assets/Characters/Wardrobe/mira_outfit_01.json"));
    EXPECT_GE(wardrobe.item_count(), 1u);
    EXPECT_NE(wardrobe.find("mira_outfit_01"), nullptr);
}

TEST(LocalizationTest, StoresAndRetrievesStrings) {
    eve::content::LocalizationTable table;
    table.set_string("game.title", "en", "Apartment Life Demo");
    table.set_string("game.title", "ja", "アパートライフ・デモ");
    EXPECT_EQ(table.get("game.title", "en"), "Apartment Life Demo");
    EXPECT_EQ(table.get("game.title", "ja"), "アパートライフ・デモ");
}

TEST(AssetImportTest, DetectsAndImportsJson) {
    eve::content::AssetImporter importer;
    const std::string source = std::string(kGameRoot) + "/project.json";
    const std::filesystem::path out = std::filesystem::temp_directory_path() / "eve_import_test";
    const eve::content::ImportResult result =
        importer.import_file(source, out.string(), eve::content::AssetType::Story);
    EXPECT_TRUE(result.success);
    std::filesystem::remove_all(out);
}

TEST(PackagingTest,ExportsTargets) {
    eve::content::ProjectPackager packager;
    eve::content::PackagingConfig config;
    config.project_name = "Apartment Life Demo";
    config.targets = {eve::content::ExportTarget::Windows, eve::content::ExportTarget::Linux,
                      eve::content::ExportTarget::SteamDeck};
    const eve::content::PackagingResult result = packager.export_project(config, kGameRoot);
    EXPECT_TRUE(result.success);
}

TEST(GameContentProjectTest, LoadsApartmentLifeDemo) {
    eve::content::GameContentProject project;
    ASSERT_TRUE(project.open(kGameRoot));
    EXPECT_EQ(project.characters().character_count(), 1u);
    EXPECT_GE(project.wardrobe().item_count(), 20u);
    EXPECT_GE(project.apartments().furniture_count(), 20u);
    EXPECT_GE(project.dialogue().package_count(), 20u);
    EXPECT_GE(project.events().event_count(), 5u);
    EXPECT_GE(project.localization().entry_count(), 1u);

    const eve::content::CharacterContentDefinition* mira = project.characters().find("mira");
    ASSERT_NE(mira, nullptr);
    EXPECT_EQ(mira->metadata.display_name, "Mira");
}

TEST(GameContentProjectTest, SaveLoadRoundTrip) {
    const std::filesystem::path index_path =
        std::filesystem::temp_directory_path() / "eve_content_index.json";
    eve::content::GameContentProject project;
    ASSERT_TRUE(project.open(kGameRoot));
    ASSERT_TRUE(project.database().save_index(index_path.string()));

    eve::content::ContentDatabase reloaded;
    EXPECT_TRUE(reloaded.load_index(index_path.string()));
    EXPECT_GE(reloaded.asset_count(), 1u);
    std::filesystem::remove(index_path);
}
