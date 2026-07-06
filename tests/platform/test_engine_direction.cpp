#include <eve/ai/relationship_simulation.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/platform/backup_manager.hpp>
#include <eve/platform/engine_mission.hpp>
#include <eve/platform/game_session.hpp>
#include <eve/platform/local_asset_library.hpp>
#include <eve/platform/mobile_input.hpp>
#include <eve/platform/project_templates.hpp>
#include <eve/platform/unified_save.hpp>
#include <eve/platform/web_publisher.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr const char* kDataRoot = EVE_DATA_DIR;
constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";
constexpr const char* kTemplatesDir = EVE_DATA_DIR "/templates";

} // namespace

TEST(EngineMissionTest, DefinesScopeBoundaries) {
    EXPECT_STREQ(eve::platform::kEngineMission,
                 "Beautiful, high-performance, anime-inspired, story-driven relationship and "
                 "life simulation.");
    EXPECT_EQ(eve::platform::kTargetFpsDesktopMin, 60u);
    EXPECT_EQ(eve::platform::kTargetFpsDesktopMax, 144u);
    EXPECT_EQ(eve::platform::kTargetFpsWeb, 60u);

    EXPECT_TRUE(eve::platform::is_in_scope_for_mission("characters"));
    EXPECT_TRUE(eve::platform::is_in_scope_for_mission("dialogue"));
    EXPECT_FALSE(eve::platform::is_in_scope_for_mission("multiplayer"));

    EXPECT_TRUE(eve::platform::is_out_of_scope(
        eve::platform::OutOfScopeFeature::MultiplayerNetworking));
    EXPECT_NE(eve::platform::out_of_scope_reason(
                  eve::platform::OutOfScopeFeature::MarketplaceInfrastructure),
              nullptr);
    EXPECT_STREQ(eve::platform::platform_name(eve::platform::SupportedPlatform::Windows),
                 "windows");
    EXPECT_STREQ(eve::platform::platform_name(eve::platform::SupportedPlatform::Web), "web");
}

TEST(UnifiedSaveTest, RoundTripAllSlices) {
    eve::platform::UnifiedSaveGame original;
    original.project_id = "apartment_life";
    original.save_slot = "slot_01";
    original.timestamp = 1'700'000'000ULL;
    original.character = {"mira", 0.6f, 0.7f, 0.8f, "happy", "living_room", "mira_outfit_01"};
    original.relationship = {0.5f, 0.6f, 0.4f, "friend"};
    original.dialogue.unlocked_nodes = {"greeting", "coffee"};
    original.dialogue.story_flags = {"met_mira"};
    original.dialogue.completed_conversations = {"intro"};
    original.memories.push_back({"rainy_evening", "Shared tea during rain", 0.8f});
    original.apartment = {"sunset_apartment", "living_room", 19.5f, "rain", 3};
    original.inventory.item_ids = {"gift_book", "camera"};
    original.wardrobe = {"mira_outfit_01", {"mira_outfit_01", "mira_outfit_02"}, {"mira_outfit_01"}};
    original.photos.push_back({"photo_01", "Screenshots/photo_01.png", "Balcony sunset", 3});
    original.story.completed_events = {"cozy_evening"};
    original.story.completed_scenes = {"movie_night"};
    original.story.unlocked_chapters = {"chapter_01"};
    original.settings = {0.9f, 0.6f, 1.0f, "en", 2, true};

    const std::filesystem::path save_path =
        std::filesystem::temp_directory_path() / "eve_unified_save_test.json";

    eve::platform::UnifiedSaveSystem save_system;
    ASSERT_TRUE(save_system.save(save_path.string(), original));

    eve::platform::UnifiedSaveGame loaded;
    ASSERT_TRUE(save_system.load(save_path.string(), loaded));
    EXPECT_EQ(loaded.project_id, original.project_id);
    EXPECT_EQ(loaded.character.character_id, "mira");
    EXPECT_NEAR(loaded.character.trust, 0.6f, 0.001f);
    EXPECT_EQ(loaded.relationship.stage, "friend");
    EXPECT_EQ(loaded.dialogue.unlocked_nodes.size(), 2u);
    EXPECT_EQ(loaded.memories.size(), 1u);
    EXPECT_EQ(loaded.apartment.weather, "rain");
    EXPECT_EQ(loaded.inventory.item_ids.size(), 2u);
    EXPECT_EQ(loaded.wardrobe.active_outfit_id, "mira_outfit_01");
    EXPECT_EQ(loaded.photos.front().caption, "Balcony sunset");
    EXPECT_EQ(loaded.story.completed_events.front(), "cozy_evening");
    EXPECT_TRUE(loaded.settings.touch_controls);

    std::filesystem::remove(save_path);
}

TEST(UnifiedSaveTest, CapturesAndAppliesRelationshipSimulation) {
    eve::ai::RelationshipSimulation sim;
    ASSERT_TRUE(sim.initialize(kDataRoot));
    sim.offer_compliment();

    eve::platform::UnifiedSaveSystem save_system;
    const eve::platform::UnifiedSaveGame captured =
        save_system.capture_from_relationship_sim(sim, "romance_demo");
    EXPECT_EQ(captured.project_id, "romance_demo");
    EXPECT_FALSE(captured.character.character_id.empty());

    eve::ai::RelationshipSimulation reloaded;
    ASSERT_TRUE(reloaded.initialize(kDataRoot));
    save_system.apply_to_relationship_sim(reloaded, captured);

    const eve::ai::CharacterSimState* original = sim.character();
    const eve::ai::CharacterSimState* loaded = reloaded.character();
    ASSERT_NE(original, nullptr);
    ASSERT_NE(loaded, nullptr);
    EXPECT_NEAR(loaded->trust, original->trust, 0.001f);
    EXPECT_NEAR(loaded->affection, original->affection, 0.001f);
}

TEST(WebPublisherTest, GeneratesDeploymentPackage) {
    const std::filesystem::path out =
        std::filesystem::temp_directory_path() / "eve_web_publish_test";

    eve::platform::WebDeployConfig config;
    config.project_name = "Apartment Life Demo";
    config.output_dir = out.string();

    eve::platform::WebPublisher publisher;
    const eve::platform::WebDeployResult result = publisher.publish(config, kGameRoot);
    ASSERT_TRUE(result.success) << result.message;
    EXPECT_TRUE(eve::FileSystem::exists(result.manifest_path));
    EXPECT_TRUE(eve::FileSystem::exists(out.string() + "/index.html"));
    EXPECT_TRUE(eve::FileSystem::exists(out.string() + "/build_info.json"));

    std::filesystem::remove_all(out);
}

TEST(MobileInputTest, OrbitCameraFromPanGesture) {
    eve::platform::MobileInputController input;
    input.on_touch_begin(0.0f, 0.0f);
    input.on_touch_move(100.0f, 50.0f);

    eve::f32 yaw = 0.0f;
    eve::f32 pitch = 0.0f;
    eve::f32 zoom = 1.0f;
    input.apply_orbit_camera(yaw, pitch, zoom);
    EXPECT_GT(yaw, 0.0f);
    EXPECT_GT(pitch, 0.0f);

    input.on_pinch(0.9f);
    input.apply_orbit_camera(yaw, pitch, zoom);
    EXPECT_LT(zoom, 1.0f);
}

TEST(BackupManagerTest, CreatesAndListsBackup) {
    const std::filesystem::path backup_root =
        std::filesystem::temp_directory_path() / "eve_backup_test";
    std::filesystem::create_directories(backup_root);

    eve::platform::BackupManager manager;
    ASSERT_TRUE(manager.create_backup(kGameRoot, backup_root.string(), "apartment_life"));

    const auto entries = manager.list_backups(backup_root.string());
    ASSERT_FALSE(entries.empty());
    EXPECT_TRUE(eve::FileSystem::is_directory(entries.front().path));

    std::filesystem::remove_all(backup_root);
}

TEST(ProjectTemplateTest, LoadsAndInstantiatesTemplate) {
    eve::platform::ProjectTemplateLibrary library;
    library.set_content_root(EVE_GAME_DIR "/..");
    ASSERT_TRUE(library.load_from_directory(kTemplatesDir));
    EXPECT_GE(library.template_count(), 1u);

    const eve::platform::ProjectTemplate* tmpl = library.find("apartment_life");
    ASSERT_NE(tmpl, nullptr);
    EXPECT_FALSE(tmpl->source_path.empty());

    const std::filesystem::path dest =
        std::filesystem::temp_directory_path() / "eve_template_instance";
    ASSERT_TRUE(library.instantiate("apartment_life", dest.string()));
    EXPECT_TRUE(eve::FileSystem::exists((dest / "project.json").string()));

    std::filesystem::remove_all(dest);
}

TEST(LocalAssetLibraryTest, ScansOfflineLibrary) {
    eve::platform::LocalAssetLibrary library;
    EXPECT_TRUE(library.scan(std::string(kGameRoot) + "/Assets"));
    EXPECT_GT(library.database().asset_count(), 0u);
}

TEST(GameSessionTest, OfflineSessionSaveAndWebPublish) {
    const std::filesystem::path save_path =
        std::filesystem::temp_directory_path() / "eve_game_session_save.json";
    const std::filesystem::path web_out =
        std::filesystem::temp_directory_path() / "eve_game_session_web";

    eve::platform::GameSession session;
    eve::platform::GameSessionConfig config;
    config.project_id = "apartment_life";
    config.data_root = kDataRoot;
    config.offline_only = true;
    ASSERT_TRUE(session.initialize(config));

    session.relationship().offer_compliment();
    ASSERT_TRUE(session.save_game(save_path.string()));

    eve::platform::GameSession reloaded;
    ASSERT_TRUE(reloaded.initialize(config));
    ASSERT_TRUE(reloaded.load_game(save_path.string()));

    const eve::ai::CharacterSimState* original = session.relationship().character();
    const eve::ai::CharacterSimState* loaded = reloaded.relationship().character();
    ASSERT_NE(original, nullptr);
    ASSERT_NE(loaded, nullptr);
    EXPECT_NEAR(loaded->trust, original->trust, 0.001f);

    ASSERT_TRUE(session.publish_web(web_out.string()));
    EXPECT_TRUE(eve::FileSystem::exists((web_out / "manifest.json").string()));

    std::filesystem::remove(save_path);
    std::filesystem::remove_all(web_out);
}
