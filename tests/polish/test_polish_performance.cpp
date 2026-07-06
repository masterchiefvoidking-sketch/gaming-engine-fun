#include <eve/gameplay/apartment_life_session.hpp>
#include <eve/polish/animation_polish.hpp>
#include <eve/polish/apartment_atmosphere.hpp>
#include <eve/polish/bug_tracker.hpp>
#include <eve/polish/camera_polish.hpp>
#include <eve/polish/dialogue_polish.hpp>
#include <eve/polish/graphics_polish.hpp>
#include <eve/polish/performance_profiler.hpp>
#include <eve/polish/validation_gate.hpp>
#include <eve/polish/wardrobe_polish.hpp>
#include <eve/rc1_release/rc1_release_app.hpp>
#include <eve/render/anime_human_material.hpp>
#include <eve/render/anime_outline_pass.hpp>
#include <eve/render/human_shader_pipeline.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr const char* kDataRoot = EVE_DATA_DIR;
constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";

} // namespace

TEST(GraphicsPolishTest, AppliesAllPresets) {
    eve::polish::GraphicsPolish polish;
    eve::render::AnimeHumanMaterial material;
    ASSERT_TRUE(material.initialize());
    eve::render::AnimeOutlinePass outline;
    eve::render::HumanShaderPipeline pipeline;
    ASSERT_TRUE(pipeline.initialize());

    for (auto preset : {eve::polish::GraphicsPreset::Low, eve::polish::GraphicsPreset::Medium,
                        eve::polish::GraphicsPreset::High, eve::polish::GraphicsPreset::Ultra,
                        eve::polish::GraphicsPreset::WebSafe,
                        eve::polish::GraphicsPreset::MobileFuture}) {
        polish.apply_preset(preset);
        polish.apply_to_material(material);
        polish.apply_to_outline(outline);
        polish.apply_to_pipeline(pipeline);
        EXPECT_TRUE(outline.settings().enabled);
    }
    material.shutdown();
    pipeline.shutdown();
}

TEST(PerformanceProfilerTest, TracksFrameBudget) {
    eve::polish::PerformanceProfiler profiler;
    profiler.set_budget({60.0f, 16.67f, 2048.0f, 5000});
    profiler.begin_frame();
    profiler.record_cpu_time(3.0f);
    profiler.record_gpu_time(6.0f);
    profiler.record_draw_calls(800);
    profiler.set_memory_usage(256 * 1024 * 1024);
    profiler.end_frame(0.016f);
    const eve::polish::PerformanceSnapshot snap = profiler.snapshot();
    EXPECT_GT(snap.fps, 0.0f);
    EXPECT_TRUE(snap.within_budget);
    EXPECT_FALSE(profiler.format_overlay().empty());
}

TEST(CameraPolishTest, RoomPresetsAndSmoothing) {
    eve::polish::CameraPolish camera;
    ASSERT_TRUE(camera.load_room_presets(std::string(kDataRoot) + "/camera/room_presets.json"));
    camera.set_active_room("living_room");
    eve::render::InspectionCamera inspection;
    EXPECT_TRUE(camera.focus_sofa_conversation(inspection));
    EXPECT_TRUE(camera.focus_photo_mode(inspection));
    EXPECT_TRUE(camera.focus_bathroom_privacy_safe(inspection));
}

TEST(AnimationPolishTest, DialogueReactions) {
    eve::animation::AnimeMotionController motion;
    eve::human::AnimeExpressionSystem expressions;
    eve::render::AnimeEyeSystem eyes;
    ASSERT_TRUE(eyes.initialize());
    expressions.load_presets(std::string(kDataRoot) + "/anime/expressions.json");
    eve::polish::AnimationPolish polish;
    polish.attach(motion, expressions, eyes);
    polish.play_idle();
    polish.play_mirror_pose();
    polish.play_dialogue_reaction("happy");
    polish.update(0.5f);
    EXPECT_FALSE(polish.is_transitioning());
}

TEST(WardrobePolishTest, SwitchAndFavorite) {
    eve::polish::WardrobePolish wardrobe;
    ASSERT_TRUE(wardrobe.initialize(kGameRoot));
    eve::human::WardrobeSystem runtime;
    ASSERT_TRUE(runtime.load_items(std::string(kDataRoot) + "/anime/wardrobe_items.json"));
    eve::human::LayeredOutfit outfit;
    EXPECT_TRUE(wardrobe.switch_outfit(runtime, outfit, "mira_outfit_01"));
    EXPECT_TRUE(wardrobe.toggle_favorite("mira_outfit_01"));
    EXPECT_GE(wardrobe.filtered_outfit_ids().size(), 1u);
}

TEST(ApartmentAtmosphereTest, TimePresets) {
    eve::world::AnimeRoomLightingSystem lighting;
    eve::polish::ApartmentAtmosphere atmosphere;
    ASSERT_TRUE(atmosphere.load(kDataRoot));
    EXPECT_TRUE(atmosphere.apply_time_preset(lighting, eve::polish::TimeOfDayPreset::Sunset));
    EXPECT_TRUE(atmosphere.apply_room("living_room", lighting));
    atmosphere.set_tv_glow(0.4f);
    EXPECT_GT(atmosphere.state().tv_glow, 0.0f);
}

TEST(DialoguePolishTest, ComplimentAndRoomComment) {
    eve::gameplay::ApartmentLifeSession session;
    eve::gameplay::ApartmentLifeConfig config;
    config.data_root = kDataRoot;
    config.game_root = kGameRoot;
    ASSERT_TRUE(session.initialize(config));
    eve::polish::DialoguePolish dialogue;
    const auto compliment = dialogue.compliment(session.relationship());
    EXPECT_TRUE(compliment.success || !compliment.rejection_reason.empty());
  session.change_room("living_room");
    const auto room = dialogue.room_comment(session.relationship(), "living_room");
    EXPECT_TRUE(room.success || !room.line.empty());
    session.shutdown();
}

TEST(ValidationGateTest, BlocksInvalidExport) {
    eve::polish::ValidationGate gate;
    const auto result =
        gate.validate_for_export(kGameRoot, eve::build::ExportProfile::WindowsDevelopment);
    EXPECT_TRUE(result.passed);
    EXPECT_TRUE(result.blocking_issues.empty());
}

TEST(BugTrackerTest, RoundTrip) {
    eve::polish::BugTracker tracker;
    eve::polish::BugReport bug;
    bug.id = "BUG-001";
    bug.severity = eve::polish::BugSeverity::Medium;
    bug.reproduction_steps = {"Launch demo", "Change outfit"};
    bug.expected_behavior = "Outfit switches instantly";
    bug.actual_behavior = "Outfit fails";
    bug.affected_platform = "Windows";
    tracker.add_bug(std::move(bug));
    EXPECT_EQ(tracker.open_count(), 1u);
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / "eve_bug_tracker.json";
    ASSERT_TRUE(tracker.save(path.string()));
    eve::polish::BugTracker loaded;
    EXPECT_TRUE(loaded.load(path.string()));
    EXPECT_TRUE(tracker.mark_fixed("BUG-001", "WardrobePolishTest.SwitchAndFavorite"));
    std::filesystem::remove(path);
}

TEST(ApartmentLifePolishTest, SessionRunsWithProfiler) {
    eve::gameplay::ApartmentLifeSession session;
    eve::gameplay::ApartmentLifeConfig config;
    config.data_root = kDataRoot;
    config.game_root = kGameRoot;
    ASSERT_TRUE(session.initialize(config));
    session.update(0.016f);
    EXPECT_GT(session.profiler().snapshot().fps, 0.0f);
    EXPECT_TRUE(session.validate_content());
    session.shutdown();
}

TEST(RC1ReleaseTest, ValidationOnlyWithoutFullExport) {
    eve::polish::ValidationGate gate;
    const auto validation =
        gate.validate_for_export(kGameRoot, eve::build::ExportProfile::WindowsDevelopment);
    EXPECT_TRUE(validation.passed);
}

TEST(MaterialLoadingTest, AnimeMaterialPresets) {
    eve::render::AnimeHumanMaterial material;
    ASSERT_TRUE(material.initialize());
    material.load_presets(std::string(kDataRoot) + "/anime/materials.json");
    EXPECT_TRUE(material.apply_preset("warm_romance"));
    material.shutdown();
}

TEST(SaveLoadPolishTest, SchemaValidation) {
    eve::polish::ValidationGate gate;
    const std::filesystem::path save = std::filesystem::temp_directory_path() / "eve_test_save.json";
    EXPECT_TRUE(gate.check_save_schema(save.string()));
}

TEST(PerformanceBudgetTest, DetectsOverBudget) {
    eve::polish::PerformanceProfiler profiler;
    profiler.set_budget({60.0f, 10.0f, 512.0f, 1000});
    profiler.begin_frame();
    profiler.record_draw_calls(5000);
    profiler.end_frame(0.05f);
    EXPECT_FALSE(profiler.check_budget());
}
