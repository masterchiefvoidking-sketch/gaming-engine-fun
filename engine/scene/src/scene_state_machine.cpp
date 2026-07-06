#include <eve/scene/scene_state_machine.hpp>

namespace eve::scene {

bool SceneStateMachine::begin_scene(std::string_view scene_id, std::string_view room_id) {
    if (state_ != SceneState::Idle && state_ != SceneState::Completed &&
        state_ != SceneState::Failed) {
        return false;
    }
    playback_ = {};
    playback_.active_scene_id = std::string(scene_id);
    playback_.active_room_id = std::string(room_id);
    playback_.elapsed_seconds = 0.0f;
    playback_.current_beat_index = 0;
    state_ = SceneState::Entering;
    return true;
}

bool SceneStateMachine::advance_beat(u32 beat_index) {
    if (state_ != SceneState::Playing && state_ != SceneState::Entering &&
        state_ != SceneState::Transitioning && state_ != SceneState::WaitingChoice) {
        return false;
    }
    playback_.current_beat_index = beat_index;
    state_ = SceneState::Playing;
    return true;
}

bool SceneStateMachine::transition_to(SceneState next) {
    state_ = next;
    return true;
}

void SceneStateMachine::complete_scene() {
    state_ = SceneState::Completed;
}

void SceneStateMachine::fail_scene(std::string_view reason) {
    failure_reason_ = std::string(reason);
    state_ = SceneState::Failed;
}

void SceneStateMachine::reset() {
    state_ = SceneState::Idle;
    playback_ = {};
    failure_reason_.clear();
}

bool SceneStateMachine::is_active() const {
    return state_ == SceneState::Loading || state_ == SceneState::Entering ||
           state_ == SceneState::Playing || state_ == SceneState::WaitingChoice ||
           state_ == SceneState::Transitioning;
}

bool SceneStateMachine::is_finished() const {
    return state_ == SceneState::Completed || state_ == SceneState::Failed;
}

} // namespace eve::scene
