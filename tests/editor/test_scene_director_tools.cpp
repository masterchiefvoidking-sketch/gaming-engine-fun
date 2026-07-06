#include <eve/editor/scene_director_tools.hpp>
#include <eve/scene/scene_director.hpp>
#include <eve/ai/relationship_simulation.hpp>

#include <gtest/gtest.h>

TEST(SceneDirectorEditorToolsTest, PanelsExposeData) {
    eve::ai::RelationshipSimulation relationship;
    ASSERT_TRUE(relationship.initialize(EVE_DATA_DIR));

    eve::scene::SceneDirector director;
    ASSERT_TRUE(director.initialize(EVE_DATA_DIR, relationship));

    eve::editor::SceneGraphEditorPanel graph_panel;
    graph_panel.set_scheduler(director.scheduler());
    EXPECT_GE(graph_panel.template_count(), 10u);

    eve::editor::ShotSequencerPanel shot_panel;
    shot_panel.set_sequencer(director.shots());
    EXPECT_GE(shot_panel.shot_count(), 10u);

    eve::editor::AnimationCueTimelinePanel anim_panel;
    anim_panel.set_system(director.animations());
    EXPECT_GE(anim_panel.cue_count(), 20u);

    eve::editor::EventConditionDebuggerPanel cond_panel;
    cond_panel.set_evaluator(director.conditions());
    eve::scene::SceneRuntimeContext ctx;
    ctx.weather = "rain";
    ctx.active_room = "living_room";
    ctx.time_of_day = 20.0f;
    ctx.stage = eve::ai::RelationshipStage::Friend;
    ctx.trust = 0.3f;
    const eve::scene::ConditionEvaluationResult preview =
        cond_panel.preview("rainy_movie_night", ctx);
    EXPECT_TRUE(preview.eligible);
}
