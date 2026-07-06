#pragma once

#include <eve/scene/scene_types.hpp>

namespace eve::scene {

class SceneStateMachine {
public:
    [[nodiscard]] SceneState state() const { return state_; }
    [[nodiscard]] ScenePlaybackState& playback() { return playback_; }
    [[nodiscard]] const ScenePlaybackState& playback() const { return playback_; }

    bool begin_scene(std::string_view scene_id, std::string_view room_id);
    bool advance_beat(u32 beat_index);
    bool transition_to(SceneState next);
    void complete_scene();
    void fail_scene(std::string_view reason);
    void reset();

    [[nodiscard]] bool is_active() const;
    [[nodiscard]] bool is_finished() const;
    [[nodiscard]] const std::string& failure_reason() const { return failure_reason_; }

private:
    SceneState state_ = SceneState::Idle;
    ScenePlaybackState playback_;
    std::string failure_reason_;
};

} // namespace eve::scene
