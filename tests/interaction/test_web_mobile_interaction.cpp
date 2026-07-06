#include <eve/interaction/browser_storage.hpp>
#include <eve/interaction/camera_input_bridge.hpp>
#include <eve/interaction/input_controller.hpp>
#include <eve/interaction/interaction_ring.hpp>
#include <eve/interaction/quality_settings.hpp>
#include <eve/interaction/responsive_ui_layout.hpp>
#include <eve/interaction/web_character_viewer.hpp>
#include <eve/interaction/web_performance_profile.hpp>
#include <eve/render/inspection_camera.hpp>
#include <eve/web_viewer/web_character_viewer_app.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr const char* kDataRoot = EVE_DATA_DIR;
constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";

} // namespace

TEST(MouseInputTest, RightDragOrbitsAndWheelZooms) {
    eve::interaction::InputController input;
    input.on_mouse_button(eve::interaction::MouseButton::Right, true);
    input.on_mouse_move(100.0f, 50.0f);
    input.on_mouse_button(eve::interaction::MouseButton::Right, false);
    input.on_mouse_scroll(2.0f);
    EXPECT_GT(input.frame().orbit_delta_yaw, 0.0f);
    EXPECT_GT(input.frame().orbit_delta_pitch, 0.0f);
    EXPECT_GT(input.frame().zoom_delta, 0.0f);
}

TEST(MouseInputTest, ShiftDragUsesFineSensitivity) {
    eve::interaction::InputController normal;
    eve::interaction::InputController fine;
    normal.on_key_shift(false);
    fine.on_key_shift(true);
    normal.on_mouse_button(eve::interaction::MouseButton::Right, true);
    fine.on_mouse_button(eve::interaction::MouseButton::Right, true);
    normal.on_mouse_move(100.0f, 0.0f);
    fine.on_mouse_move(100.0f, 0.0f);
    EXPECT_LT(fine.frame().orbit_delta_yaw, normal.frame().orbit_delta_yaw);
}

TEST(MouseInputTest, MiddleDragPans) {
    eve::interaction::InputController input;
    input.on_mouse_button(eve::interaction::MouseButton::Middle, true);
    input.on_mouse_move(40.0f, 20.0f);
    EXPECT_GT(input.frame().pan_delta.x, 0.0f);
}

TEST(TouchInputTest, PanOrbitsPinchZoomsAndLongPressOpensContext) {
    eve::interaction::InputController input;
    input.on_touch_begin(0.0f, 0.0f);
    input.on_touch_move(80.0f, 40.0f);
    EXPECT_GT(input.frame().orbit_delta_yaw, 0.0f);
    input.on_pinch(0.8f);
    EXPECT_GT(input.frame().zoom_delta, 0.0f);
    input.on_two_finger_pan(10.0f, 5.0f);
    EXPECT_GT(input.frame().pan_delta.x, 0.0f);
    input.on_touch_long_press(120.0f, 200.0f);
    EXPECT_TRUE(input.frame().context_menu_pressed);
}

TEST(CameraBridgeTest, OrbitZoomPan) {
    eve::render::InspectionCamera camera;
    eve::interaction::CameraInputBridge bridge;
    eve::interaction::InputFrame frame;
    frame.orbit_delta_yaw = 0.2f;
    frame.orbit_delta_pitch = 0.1f;
    frame.zoom_delta = 0.5f;
    frame.pan_delta = {0.1f, 0.05f};
    const eve::math::Vec3 before = camera.focus();
    bridge.apply(camera, 0.016f, frame);
    EXPECT_NE(camera.focus().x, before.x);
    EXPECT_GT(bridge.last_orbit_input().orbit_delta_yaw, 0.0f);
}

TEST(CameraBridgeTest, BodyRegionFocus) {
    eve::render::InspectionCamera camera;
    camera.focus_body_region({0.0f, 1.5f, 0.0f});
    EXPECT_EQ(camera.mode(), eve::render::CameraMode::BodyRegionFocus);
}

TEST(InteractionRingTest, FiltersByRelationshipAndMood) {
    eve::interaction::InteractionRing ring;
    eve::interaction::InteractionRingContext context;
    context.is_character = true;
    context.interaction.mood = "upset";

    const auto actions = ring.build_actions(context);
    const eve::interaction::InteractionRingAction* talk = nullptr;
    const eve::interaction::InteractionRingAction* compliment = nullptr;
    for (const auto& action : actions) {
        if (action.type == eve::interaction::RingActionType::Talk) {
            talk = &action;
        }
        if (action.type == eve::interaction::RingActionType::Compliment) {
            compliment = &action;
        }
    }
    ASSERT_NE(talk, nullptr);
    ASSERT_NE(compliment, nullptr);
    EXPECT_TRUE(talk->enabled);
    EXPECT_FALSE(compliment->enabled);
}

TEST(InteractionRingTest, HighTrustEnablesRomanceActions) {
    eve::interaction::InteractionRing ring;
    eve::interaction::InteractionRingContext context;
    context.is_character = true;
    context.interaction.trust = 0.8f;
    context.interaction.relationship = 0.5f;
    context.interaction.privacy_level = 1.0f;
    EXPECT_TRUE(ring.is_action_available(eve::interaction::RingActionType::Compliment, context));
    EXPECT_TRUE(ring.is_action_available(eve::interaction::RingActionType::ChangeOutfit, context));
}

TEST(ResponsiveUILayoutTest, AdaptsDesktopTabletPhone) {
    eve::interaction::ResponsiveUILayout layout;
    layout.set_viewport(1920.0f, 1080.0f);
    EXPECT_EQ(layout.profile, eve::interaction::UILayoutProfile::Desktop);
    EXPECT_GE(layout.touch_target_size(), 36.0f);

    layout.set_viewport(1024.0f, 768.0f);
    EXPECT_EQ(layout.profile, eve::interaction::UILayoutProfile::Tablet);

    layout.set_viewport(390.0f, 844.0f);
    EXPECT_EQ(layout.profile, eve::interaction::UILayoutProfile::Phone);
    EXPECT_GE(layout.touch_target_size(), 48.0f);
}

TEST(QualitySettingsTest, TierAndBatterySaver) {
    eve::interaction::QualitySettings settings;
    settings.apply_tier(eve::interaction::QualityTier::Low);
    EXPECT_LT(settings.resolution_scale, 1.0f);
    EXPECT_FALSE(settings.bloom);
    settings.apply_battery_saver();
    EXPECT_TRUE(settings.battery_saver);
    EXPECT_LE(settings.frame_cap, 60u);
}

TEST(WebPerformanceTest, SyncsMobileSettings) {
    eve::interaction::WebPerformanceProfile profile;
    profile.apply_mobile_defaults();
    EXPECT_TRUE(profile.mobile.battery_saver);
    EXPECT_GE(profile.animation_lod, 1u);
}

TEST(BrowserStorageTest, RoundTripUnifiedSave) {
    const std::filesystem::path root = std::filesystem::temp_directory_path() / "eve_browser_storage";
    eve::interaction::LocalFileStorage desktop((root / "desktop").string());
    eve::interaction::BrowserIndexedDbStorage browser((root / "web").string());
    eve::platform::UnifiedSaveSystem save_system;
    eve::platform::UnifiedSaveGame game;
    game.project_id = "viewer";
    game.character.character_id = "mira";

    eve::interaction::SaveStorageRouter router;
    router.set_desktop_storage(std::move(desktop));
    router.set_browser_storage(std::move(browser));

    router.set_active_backend(eve::interaction::StorageBackend::LocalFile);
    ASSERT_TRUE(router.save_game(save_system, game, "slot_a"));
    eve::platform::UnifiedSaveGame loaded;
    ASSERT_TRUE(router.load_game(save_system, loaded, "slot_a"));
    EXPECT_EQ(loaded.project_id, "viewer");

    router.set_active_backend(eve::interaction::StorageBackend::BrowserIndexedDb);
    ASSERT_TRUE(router.save_game(save_system, game, "slot_b"));
    loaded = {};
    ASSERT_TRUE(router.load_game(save_system, loaded, "slot_b"));
    EXPECT_EQ(loaded.character.character_id, "mira");

    std::filesystem::remove_all(root);
}

TEST(WebCharacterViewerTest, VerticalSliceFlow) {
    eve::interaction::WebCharacterViewer viewer;
    eve::interaction::WebCharacterViewerConfig config;
    config.data_root = kDataRoot;
    config.game_root = kGameRoot;
    ASSERT_TRUE(viewer.initialize(config));

    viewer.input().on_touch_move(50.0f, 25.0f);
    viewer.update(0.016f);
    EXPECT_TRUE(viewer.handle_select());
    EXPECT_TRUE(viewer.open_interaction_ring());
    EXPECT_TRUE(viewer.change_outfit("mira_outfit_01"));
    EXPECT_TRUE(viewer.change_hair("mira_hair_bun"));
    EXPECT_TRUE(viewer.change_expression("soft_smile"));
    EXPECT_TRUE(viewer.execute_ring_action(eve::interaction::RingActionType::Talk));
    EXPECT_TRUE(viewer.execute_ring_action(eve::interaction::RingActionType::Compliment));
    EXPECT_TRUE(viewer.enter_photo_mode(true));
    EXPECT_TRUE(viewer.save_session());
    EXPECT_TRUE(viewer.load_session());

    const std::filesystem::path web_out = std::filesystem::temp_directory_path() / "eve_viewer_web";
    EXPECT_TRUE(viewer.publish_web(web_out.string()));
    std::filesystem::remove_all(web_out);
    viewer.shutdown();
}

TEST(WebCharacterViewerAppTest, RunsSlice) {
    eve::web_viewer::WebCharacterViewerApp app;
    ASSERT_TRUE(app.initialize(kDataRoot, kGameRoot));
    EXPECT_TRUE(app.run_vertical_slice_flow());
    app.shutdown();
}
