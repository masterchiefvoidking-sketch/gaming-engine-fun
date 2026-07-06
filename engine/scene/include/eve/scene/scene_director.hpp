#pragma once

#include <eve/ai/boundaries.hpp>
#include <eve/ai/relationship_simulation.hpp>
#include <eve/animation/anime_motion.hpp>
#include <eve/human/anime_expression_system.hpp>
#include <eve/render/inspection_camera.hpp>
#include <eve/render/romance_camera.hpp>
#include <eve/scene/animation_cue_system.hpp>
#include <eve/scene/camera_shot_sequencer.hpp>
#include <eve/scene/dialogue_scene_sequencer.hpp>
#include <eve/scene/emotional_outcomes.hpp>
#include <eve/scene/event_scheduler.hpp>
#include <eve/scene/lighting_cue_system.hpp>
#include <eve/scene/music_cue_system.hpp>
#include <eve/scene/room_event_triggers.hpp>
#include <eve/scene/scene_conditions.hpp>
#include <eve/scene/scene_state_machine.hpp>
#include <eve/world/anime_room_lighting.hpp>

#include <string>
#include <vector>

namespace eve::scene {

struct SceneStartResult {
    bool success = false;
    std::string scene_id;
    std::string rejection_reason;
};

struct SceneStepResult {
    bool advanced = false;
    bool completed = false;
    std::string dialogue_line;
    std::string camera_shot_id;
    std::string animation_cue_id;
    std::string lighting_cue_id;
    std::string music_track_id;
    std::string memory_id;
    std::string state;
};

struct SceneDirectorState {
    SchedulerContext scheduler;
    std::vector<std::string> completed_scenes;
    std::vector<std::string> unlocked_dialogue;
    std::vector<std::string> unlocked_events;
};

class SceneDirector {
public:
    bool initialize(std::string_view data_root, ai::RelationshipSimulation& relationship_sim);

    SceneStartResult start_scene(std::string_view scene_id);
    SceneStartResult start_from_room(std::string_view room_id, std::string_view interaction_id);
    SceneStepResult advance(f32 delta_seconds);
    SceneStepResult invite_to_scene(std::string_view scene_id);
    bool make_dialogue_choice(std::string_view node_id, std::string_view choice_id);

    bool save_state(std::string_view path) const;
    bool load_state(std::string_view path);

    [[nodiscard]] SceneRuntimeContext build_runtime_context() const;
    [[nodiscard]] const SceneStateMachine& state_machine() const { return state_machine_; }
    [[nodiscard]] const SceneDirectorState& director_state() const { return director_state_; }
    [[nodiscard]] SceneDirectorState& director_state() { return director_state_; }
    [[nodiscard]] const SceneTemplate* active_template() const;

    EventScheduler& scheduler() { return scheduler_; }
    SceneConditionEvaluator& conditions() { return conditions_; }
    CameraShotSequencer& shots() { return shots_; }
    AnimationCueSystem& animations() { return animations_; }

    void attach_camera(render::InspectionCamera& camera);
    void attach_lighting(world::AnimeRoomLightingSystem& lighting);
    void attach_motion(animation::AnimeMotionController& motion);
    void attach_expressions(human::AnimeExpressionSystem& expressions);

private:
    bool play_beat(const SceneBeat& beat);
    bool apply_privacy_gate(const SceneTemplate& tmpl) const;

    ai::RelationshipSimulation* relationship_sim_ = nullptr;
    SceneStateMachine state_machine_;
    SceneDirectorState director_state_;
    EventScheduler scheduler_;
    SceneConditionEvaluator conditions_;
    RoomEventTriggerSystem room_triggers_;
    EmotionalOutcomeSystem outcomes_;
    CameraShotSequencer shots_;
    AnimationCueSystem animations_;
    LightingCueSystem lighting_cues_;
    MusicCueSystem music_cues_;
    DialogueSceneSequencer dialogue_sequencer_;
    ai::BoundaryConsentSystem boundaries_;

    render::RomanceCameraController romance_camera_;
    render::InspectionCamera* inspection_camera_ = nullptr;
    world::AnimeRoomLightingSystem* lighting_ = nullptr;
    animation::AnimeMotionController* motion_ = nullptr;
    human::AnimeExpressionSystem* expressions_ = nullptr;

    const SceneTemplate* active_template_ = nullptr;
    std::string data_root_;
    bool initialized_ = false;
};

} // namespace eve::scene
