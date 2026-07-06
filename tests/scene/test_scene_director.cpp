#include <eve/ai/relationship_simulation.hpp>
#include <eve/animation/anime_motion.hpp>
#include <eve/human/anime_expression_system.hpp>
#include <eve/movie_demo/rainy_movie_night_demo.hpp>
#include <eve/scene/animation_cue_system.hpp>
#include <eve/scene/camera_shot_sequencer.hpp>
#include <eve/scene/emotional_outcomes.hpp>
#include <eve/scene/event_scheduler.hpp>
#include <eve/scene/scene_conditions.hpp>
#include <eve/scene/scene_director.hpp>
#include <eve/scene/scene_state_machine.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr const char* kDataRoot = EVE_DATA_DIR;
constexpr const char* kScenePath = EVE_DATA_DIR "/scene_director";

} // namespace

TEST(SceneConditionTest, EvaluatesRainyMovieCondition) {
    eve::scene::SceneConditionEvaluator evaluator;
    ASSERT_TRUE(evaluator.load_conditions(std::string(kScenePath) + "/event_conditions.json"));
    EXPECT_GE(evaluator.condition_count(), 15u);

    eve::scene::SceneRuntimeContext ctx;
    ctx.weather = "rain";
    ctx.active_room = "living_room";
    ctx.time_of_day = 20.0f;
    ctx.stage = eve::ai::RelationshipStage::Friend;
    ctx.trust = 0.3f;
    ctx.consent_granted = true;

    const eve::scene::ConditionEvaluationResult result =
        evaluator.evaluate("rainy_movie_night", ctx);
    EXPECT_TRUE(result.eligible);
}

TEST(SceneConditionTest, RejectsWrongWeather) {
    eve::scene::SceneConditionEvaluator evaluator;
    ASSERT_TRUE(evaluator.load_conditions(std::string(kScenePath) + "/event_conditions.json"));

    eve::scene::SceneRuntimeContext ctx;
    ctx.weather = "clear";
    ctx.active_room = "living_room";
    ctx.time_of_day = 20.0f;
    ctx.stage = eve::ai::RelationshipStage::Friend;

    const eve::scene::ConditionEvaluationResult result =
        evaluator.evaluate("rainy_movie_night", ctx);
    EXPECT_FALSE(result.eligible);
}

TEST(EventSchedulerTest, FindsEligibleEvents) {
    eve::scene::EventScheduler scheduler;
    eve::scene::SceneConditionEvaluator conditions;
    ASSERT_TRUE(scheduler.load_templates(std::string(kScenePath) + "/scene_templates.json"));
    ASSERT_TRUE(conditions.load_conditions(std::string(kScenePath) + "/event_conditions.json"));
    EXPECT_GE(scheduler.template_count(), 10u);

    eve::scene::SchedulerContext ctx;
    ctx.runtime.weather = "rain";
    ctx.runtime.active_room = "living_room";
    ctx.runtime.time_of_day = 20.0f;
    ctx.runtime.stage = eve::ai::RelationshipStage::Friend;
    ctx.runtime.trust = 0.3f;
    ctx.current_hour = 20.0f;

    const auto candidates = scheduler.eligible_events(ctx, conditions);
    EXPECT_FALSE(candidates.empty());
}

TEST(EventSchedulerTest, CooldownBlocksRepeat) {
    eve::scene::EventScheduler scheduler;
    eve::scene::SchedulerContext ctx;
    ctx.current_hour = 20.0f;
    scheduler.set_cooldown("rainy_movie_night", 24.0f, ctx);
    EXPECT_TRUE(scheduler.is_on_cooldown("rainy_movie_night", ctx));
}

TEST(SceneStateMachineTest, TransitionsThroughStates) {
    eve::scene::SceneStateMachine machine;
    EXPECT_TRUE(machine.begin_scene("test_scene", "living_room"));
    EXPECT_TRUE(machine.is_active());
    machine.transition_to(eve::scene::SceneState::Playing);
    machine.advance_beat(1);
    machine.complete_scene();
    EXPECT_TRUE(machine.is_finished());
}

TEST(CameraShotSequencerTest, LoadsAndPlaysShots) {
    eve::scene::CameraShotSequencer sequencer;
    ASSERT_TRUE(sequencer.load_shots(std::string(kScenePath) + "/camera_shots.json"));
    EXPECT_GE(sequencer.shot_count(), 10u);

    eve::render::InspectionCamera camera;
    eve::render::RomanceCameraController romance_camera;
    romance_camera.attach(camera);
    EXPECT_TRUE(sequencer.play_shot("wide_living_room", romance_camera));
    sequencer.update(1.0f);
    EXPECT_FALSE(sequencer.playback().completed);
    sequencer.update(5.0f);
    EXPECT_TRUE(sequencer.playback().completed);
}

TEST(AnimationCueSystemTest, DispatchesCues) {
    eve::scene::AnimationCueSystem cues;
    ASSERT_TRUE(cues.load_cues(std::string(kScenePath) + "/animation_cues.json"));
    EXPECT_GE(cues.cue_count(), 20u);

    eve::animation::AnimeMotionController motion;
    eve::human::AnimeExpressionSystem expressions;
    const eve::scene::CueDispatchResult result = cues.dispatch("smile", motion, expressions);
    EXPECT_TRUE(result.dispatched);
    EXPECT_EQ(result.cue_id, "smile");
}

TEST(EmotionalOutcomeTest, AppliesStatDeltas) {
    eve::scene::EmotionalOutcomeSystem outcomes;
    eve::ai::MemorySystem memories;
    ASSERT_TRUE(outcomes.load_outcomes(std::string(kScenePath) + "/emotional_outcomes.json"));
    memories.load_templates(EVE_DATA_DIR "/romance_sim/memory_templates.json");

    eve::ai::CharacterSimState character;
    eve::ai::RelationshipModel relationship;
    character.trust = 0.2f;
    character.affection = 0.2f;

    const eve::scene::OutcomeApplicationResult result =
        outcomes.apply("movie_night_memory", character, relationship, memories, 1);
    EXPECT_TRUE(result.applied);
    EXPECT_GT(character.affection, 0.2f);
    EXPECT_FALSE(result.memory_id.empty());
}

TEST(SceneDirectorTest, StartsRainyMovieNight) {
    eve::ai::RelationshipSimulation relationship;
    ASSERT_TRUE(relationship.initialize(kDataRoot));

    eve::scene::SceneDirector director;
    ASSERT_TRUE(director.initialize(kDataRoot, relationship));

    eve::ai::CharacterSimState* character = relationship.character();
    ASSERT_NE(character, nullptr);
    character->current_room_id = "living_room";
    character->trust = 0.25f;
    character->familiarity = 0.2f;
    character->affection = 0.15f;
    director.director_state().scheduler.runtime.weather = "rain";
    director.director_state().scheduler.runtime.time_of_day = 20.0f;

    const eve::scene::SceneStartResult started =
        director.start_scene("rainy_movie_night");
    EXPECT_TRUE(started.success) << started.rejection_reason;

    bool completed = false;
    for (int i = 0; i < 15; ++i) {
        const eve::scene::SceneStepResult step = director.advance(1.0f);
        if (step.completed) {
            completed = true;
            break;
        }
    }
    EXPECT_TRUE(completed);
}

TEST(SceneDirectorTest, SaveLoadSceneState) {
    const std::filesystem::path save_path =
        std::filesystem::temp_directory_path() / "eve_scene_director_test.json";

    eve::ai::RelationshipSimulation relationship;
    ASSERT_TRUE(relationship.initialize(kDataRoot));

    eve::scene::SceneDirector director;
    ASSERT_TRUE(director.initialize(kDataRoot, relationship));

    eve::ai::CharacterSimState* character = relationship.character();
    ASSERT_NE(character, nullptr);
    character->trust = 0.2f;
    character->familiarity = 0.15f;
    character->current_room_id = "living_room";
    director.director_state().scheduler.runtime.time_of_day = 19.0f;

    const eve::scene::SceneStartResult started = director.start_scene("living_room_relax");
    ASSERT_TRUE(started.success) << started.rejection_reason;
    for (int i = 0; i < 5; ++i) {
        director.advance(1.0f);
    }
    ASSERT_TRUE(director.save_state(save_path.string()));

    eve::scene::SceneDirector reloaded;
    ASSERT_TRUE(reloaded.initialize(kDataRoot, relationship));
    EXPECT_TRUE(reloaded.load_state(save_path.string()));
    EXPECT_GE(reloaded.director_state().completed_scenes.size(), 1u);

    std::filesystem::remove(save_path);
}

TEST(SceneDirectorTest, PrivacyGateBlocksBedroom) {
    eve::ai::RelationshipSimulation relationship;
    ASSERT_TRUE(relationship.initialize(kDataRoot));

    eve::scene::SceneDirector director;
    ASSERT_TRUE(director.initialize(kDataRoot, relationship));

    eve::ai::CharacterSimState* character = relationship.character();
    ASSERT_NE(character, nullptr);
    character->privacy_level = eve::ai::PrivacyLevel::Public;

    const eve::scene::SceneStartResult started = director.start_scene("bedroom_goodnight");
    EXPECT_FALSE(started.success);
}

TEST(RainyMovieNightDemoTest, RunsVerticalSlice) {
    eve::movie_demo::RainyMovieNightDemo demo;
    ASSERT_TRUE(demo.initialize(kDataRoot));

    const auto begin = demo.begin_scene();
    EXPECT_TRUE(begin.success) << begin.dialogue;

    bool completed = false;
    for (int i = 0; i < 15; ++i) {
        const auto beat = demo.advance_scene();
        if (beat.state == "completed") {
            completed = true;
            break;
        }
    }
    EXPECT_TRUE(completed);

    const std::filesystem::path save_path =
        std::filesystem::temp_directory_path() / "eve_movie_night_test.json";
    EXPECT_TRUE(demo.save_state(save_path.string()));

    eve::movie_demo::RainyMovieNightDemo reloaded;
    ASSERT_TRUE(reloaded.initialize(kDataRoot));
    EXPECT_TRUE(reloaded.load_state(save_path.string()));

    const auto recall = reloaded.recall_movie_memory();
    EXPECT_TRUE(recall.success);

    std::filesystem::remove(save_path);
    std::filesystem::remove(std::string(save_path) + ".rel");
    demo.shutdown();
}
