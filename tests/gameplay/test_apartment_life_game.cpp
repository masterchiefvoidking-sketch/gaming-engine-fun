#include <eve/apartment_demo/apartment_life_demo_app.hpp>
#include <eve/gameplay/apartment_life_session.hpp>
#include <eve/interaction/camera_input_bridge.hpp>
#include <eve/interaction/input_controller.hpp>
#include <eve/render/inspection_camera.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr const char* kDataRoot = EVE_DATA_DIR;
constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";

eve::gameplay::ApartmentLifeConfig make_config() {
    eve::gameplay::ApartmentLifeConfig config;
    config.data_root = kDataRoot;
    config.game_root = kGameRoot;
    config.save_slot = "test_slot";
    return config;
}

} // namespace

TEST(ApartmentLifeSessionTest, InitializesWithMiraContent) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    EXPECT_TRUE(session.initialized());
    EXPECT_EQ(session.content().characters().character_count(), 1u);
    EXPECT_GE(session.content().wardrobe().item_count(), 20u);
    EXPECT_EQ(session.config().character_id, "mira");
    session.shutdown();
}

TEST(ApartmentLifeSessionTest, LoadsAllSixRooms) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    EXPECT_EQ(session.room_ids().size(), 6u);
    for (const char* room :
         {"living_room", "kitchen", "bedroom", "bathroom", "dining_room", "wardrobe"}) {
        EXPECT_TRUE(session.change_room(room));
        EXPECT_EQ(session.current_room(), room);
    }
    session.shutdown();
}

TEST(CharacterCustomizationTest, OutfitHairExpression) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    EXPECT_TRUE(session.change_outfit("mira_outfit_01"));
    EXPECT_TRUE(session.change_hair("mira_hair_bun"));
    EXPECT_TRUE(session.change_expression("soft_smile"));
    EXPECT_TRUE(session.change_outfit("mira_outfit_05"));
    session.shutdown();
}

TEST(WardrobeCompatibilityTest, SwapsBetweenOutfits) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    EXPECT_TRUE(session.change_outfit("mira_outfit_01"));
    EXPECT_TRUE(session.change_outfit("mira_outfit_03"));
    EXPECT_TRUE(session.change_outfit("mira_outfit_07"));
    EXPECT_EQ(session.viewer().inspection().preview().outfit_id, "mira_outfit_07");
    session.shutdown();
}

TEST(CameraControlsTest, OrbitZoomPan) {
    eve::render::InspectionCamera camera;
    eve::interaction::CameraInputBridge bridge;
    eve::interaction::InputFrame frame;
    frame.orbit_delta_yaw = 0.3f;
    frame.zoom_delta = 0.4f;
    frame.pan_delta = {0.1f, 0.05f};
    bridge.apply(camera, 0.016f, frame);
    EXPECT_GT(bridge.last_orbit_input().orbit_delta_yaw, 0.0f);
}

TEST(DialogueBranchingTest, MiraGreetingTalk) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    session.change_room("living_room");
    EXPECT_TRUE(session.talk("greet_evening"));
    EXPECT_TRUE(session.talk("room_living_room_1"));
    session.shutdown();
}

TEST(RelationshipUpdateTest, ComplimentAndEvent) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    const auto before = session.relationship().character()->affection;
    session.viewer().execute_ring_action(eve::interaction::RingActionType::Compliment);
    EXPECT_GE(session.relationship().character()->affection, before);
    session.shutdown();
}

TEST(EventConditionsTest, TriggersDemoEvents) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    const eve::gameplay::EventPlayResult coffee = session.trigger_event("coffee_together");
    EXPECT_TRUE(coffee.success || !coffee.rejection_reason.empty());
    session.shutdown();
}

TEST(SceneCooldownTest, RepeatedEventMayReject) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    const auto first = session.trigger_event("morning_greeting");
    const auto second = session.trigger_event("morning_greeting");
    EXPECT_TRUE(first.success);
    EXPECT_TRUE(second.success || !second.rejection_reason.empty());
    session.shutdown();
}

TEST(SaveLoadTest, RoundTripSession) {
    eve::gameplay::ApartmentLifeSession session;
    auto config = make_config();
    config.save_slot = "gameplay_test_slot";
    ASSERT_TRUE(session.initialize(config));
    session.change_room("kitchen");
    session.change_outfit("mira_outfit_03");
    ASSERT_TRUE(session.save_game());
    session.change_room("living_room");
    ASSERT_TRUE(session.load_game());
    EXPECT_EQ(session.current_room(), "kitchen");
    session.shutdown();
    std::filesystem::remove(std::string(kDataRoot) + "/saves/gameplay_test_slot.json");
}

TEST(AssetValidationTest, ContentPassesValidator) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    EXPECT_TRUE(session.validate_content());
    session.shutdown();
}

TEST(MissingAssetDetectionTest, RejectsInvalidGameRoot) {
    eve::gameplay::ApartmentLifeSession session;
    auto config = make_config();
    config.game_root = "/nonexistent/game/path";
    EXPECT_FALSE(session.initialize(config));
}

TEST(WebExportTest, ExportsWebDevelopmentBuild) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    std::string output;
    EXPECT_TRUE(session.export_web_dev(output));
    EXPECT_FALSE(output.empty());
    session.shutdown();
}

TEST(WindowsExportTest, ExportsWindowsDevelopmentBuild) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    std::string output;
    EXPECT_TRUE(session.export_windows_dev(output));
    EXPECT_FALSE(output.empty());
    session.shutdown();
}

TEST(ApartmentLifeDemoAppTest, RunsVerticalSlice) {
    eve::apartment_demo::ApartmentLifeDemoApp app;
    ASSERT_TRUE(app.initialize(kDataRoot, kGameRoot));
    EXPECT_TRUE(app.run_vertical_slice());
    app.shutdown();
}

TEST(ApartmentLifeVerticalSliceTest, FullPlayableLoop) {
    eve::gameplay::ApartmentLifeSession session;
    ASSERT_TRUE(session.initialize(make_config()));
    const auto steps = session.run_vertical_slice();
    ASSERT_FALSE(steps.empty());
    EXPECT_TRUE(steps.front().success);
    bool save_ok = false;
    bool load_ok = false;
    for (const auto& step : steps) {
        if (step.phase == "save") {
            save_ok = step.success;
        }
        if (step.phase == "load") {
            load_ok = step.success;
        }
    }
    EXPECT_TRUE(save_ok);
    EXPECT_TRUE(load_ok);
    session.shutdown();
    std::filesystem::remove(std::string(kDataRoot) + "/saves/test_slot.json");
}
