#include <algorithm>
#include <eve/animation/state_machine.hpp>

namespace eve::animation {

void AnimationStateMachine::add_state(AnimationState state) {
    states_[state.id] = std::move(state);
}

void AnimationStateMachine::add_transition(AnimationTransition transition) {
    transitions_.push_back(std::move(transition));
}

void AnimationStateMachine::set_initial(AnimationStateId state) {
    current_ = state;
    target_ = state;
    blend_weight_ = 1.0f;
}

void AnimationStateMachine::update(f32 delta_seconds,
                                   const std::unordered_map<std::string, bool>& conditions) {
    for (const AnimationTransition& transition : transitions_) {
        if (transition.from != current_) {
            continue;
        }
        const auto it = conditions.find(transition.condition);
        if (it != conditions.end() && it->second) {
            target_ = transition.to;
            blend_duration_ = transition.blend_duration;
            blend_timer_ = 0.0f;
            blend_weight_ = 0.0f;
            break;
        }
    }

    if (target_ != current_) {
        blend_timer_ += delta_seconds;
        blend_weight_ =
            blend_duration_ > 0.0f ? std::min(blend_timer_ / blend_duration_, 1.0f) : 1.0f;
        if (blend_weight_ >= 1.0f) {
            current_ = target_;
        }
    }
}

const AnimationState* AnimationStateMachine::find_state(AnimationStateId id) const {
    const auto it = states_.find(id);
    if (it == states_.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace eve::animation
