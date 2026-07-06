#pragma once

#include <eve/core/types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::animation {

enum class AnimationStateId : u32 {
    Idle = 1,
    Walk,
    Sit,
    Interact,
    Facial,
    Breathing,
};

struct AnimationTransition {
    AnimationStateId from = AnimationStateId::Idle;
    AnimationStateId to = AnimationStateId::Idle;
    std::string condition;
    f32 blend_duration = 0.2f;
};

struct AnimationState {
    AnimationStateId id = AnimationStateId::Idle;
    std::string clip_id;
    bool looping = true;
    f32 playback_speed = 1.0f;
    u32 layer = 0;
};

class AnimationStateMachine {
public:
    void add_state(AnimationState state);
    void add_transition(AnimationTransition transition);
    void set_initial(AnimationStateId state);

    void update(f32 delta_seconds, const std::unordered_map<std::string, bool>& conditions);
    [[nodiscard]] AnimationStateId current_state() const { return current_; }
    [[nodiscard]] f32 blend_weight() const { return blend_weight_; }
    [[nodiscard]] const AnimationState* find_state(AnimationStateId id) const;

private:
    AnimationStateId current_ = AnimationStateId::Idle;
    AnimationStateId target_ = AnimationStateId::Idle;
    f32 blend_weight_ = 1.0f;
    f32 blend_timer_ = 0.0f;
    f32 blend_duration_ = 0.0f;
    std::unordered_map<AnimationStateId, AnimationState> states_;
    std::vector<AnimationTransition> transitions_;
};

} // namespace eve::animation
