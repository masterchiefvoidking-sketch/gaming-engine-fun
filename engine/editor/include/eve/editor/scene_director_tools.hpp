#pragma once

#include <eve/scene/animation_cue_system.hpp>
#include <eve/scene/camera_shot_sequencer.hpp>
#include <eve/scene/event_scheduler.hpp>
#include <eve/scene/scene_conditions.hpp>
#include <eve/scene/scene_director.hpp>

#include <string>

namespace eve::editor {

class SceneGraphEditorPanel {
public:
    void set_scheduler(const scene::EventScheduler& scheduler);
    [[nodiscard]] std::size_t template_count() const { return template_count_; }
    void select_scene(std::string_view scene_id) { selected_scene_id_ = std::string(scene_id); }
    [[nodiscard]] const std::string& selected_scene_id() const { return selected_scene_id_; }

private:
    std::size_t template_count_ = 0;
    std::string selected_scene_id_;
};

class ShotSequencerPanel {
public:
    void set_sequencer(const scene::CameraShotSequencer& sequencer);
    [[nodiscard]] std::size_t shot_count() const { return shot_count_; }

private:
    std::size_t shot_count_ = 0;
};

class DialogueSceneEditorPanel {
public:
    void set_director(const scene::SceneDirector& director);
    [[nodiscard]] const std::string& active_scene_id() const { return active_scene_id_; }

private:
    std::string active_scene_id_;
};

class AnimationCueTimelinePanel {
public:
    void set_system(const scene::AnimationCueSystem& cues);
    [[nodiscard]] std::size_t cue_count() const { return cue_count_; }

private:
    std::size_t cue_count_ = 0;
};

class LightingCueTimelinePanel {
public:
    void set_cue_count(std::size_t count) { cue_count_ = count; }
    [[nodiscard]] std::size_t cue_count() const { return cue_count_; }

private:
    std::size_t cue_count_ = 0;
};

class MusicCueTimelinePanel {
public:
    void set_cue_count(std::size_t count) { cue_count_ = count; }
    [[nodiscard]] std::size_t cue_count() const { return cue_count_; }

private:
    std::size_t cue_count_ = 0;
};

class EventConditionDebuggerPanel {
public:
    void set_evaluator(const scene::SceneConditionEvaluator& evaluator);
    scene::ConditionEvaluationResult preview(std::string_view condition_id,
                                             const scene::SceneRuntimeContext& ctx) const;

private:
    const scene::SceneConditionEvaluator* evaluator_ = nullptr;
};

class SceneOutcomePreviewPanel {
public:
    void set_outcome_count(std::size_t count) { outcome_count_ = count; }
    [[nodiscard]] std::size_t outcome_count() const { return outcome_count_; }

private:
    std::size_t outcome_count_ = 0;
};

class RoomTriggerEditorPanel {
public:
    void set_trigger_count(std::size_t count) { trigger_count_ = count; }
    [[nodiscard]] std::size_t trigger_count() const { return trigger_count_; }

private:
    std::size_t trigger_count_ = 0;
};

} // namespace eve::editor
