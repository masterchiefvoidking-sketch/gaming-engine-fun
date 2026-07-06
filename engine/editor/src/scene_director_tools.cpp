#include <eve/editor/scene_director_tools.hpp>

namespace eve::editor {

void SceneGraphEditorPanel::set_scheduler(const scene::EventScheduler& scheduler) {
    template_count_ = scheduler.template_count();
}

void ShotSequencerPanel::set_sequencer(const scene::CameraShotSequencer& sequencer) {
    shot_count_ = sequencer.shot_count();
}

void DialogueSceneEditorPanel::set_director(const scene::SceneDirector& director) {
    if (const scene::SceneTemplate* tmpl = director.active_template()) {
        active_scene_id_ = tmpl->id;
    } else if (!director.director_state().completed_scenes.empty()) {
        active_scene_id_ = director.director_state().completed_scenes.back();
    }
}

void AnimationCueTimelinePanel::set_system(const scene::AnimationCueSystem& cues) {
    cue_count_ = cues.cue_count();
}

void EventConditionDebuggerPanel::set_evaluator(const scene::SceneConditionEvaluator& evaluator) {
    evaluator_ = &evaluator;
}

scene::ConditionEvaluationResult EventConditionDebuggerPanel::preview(
    std::string_view condition_id, const scene::SceneRuntimeContext& ctx) const {
    if (evaluator_ == nullptr) {
        return {};
    }
    return evaluator_->evaluate(condition_id, ctx);
}

} // namespace eve::editor
