#include <eve/polish/animation_polish.hpp>

namespace eve::animation {
enum class AnimeBodyLanguageId : u8;
}

namespace eve::polish {

void AnimationPolish::attach(animation::AnimeMotionController& motion,
                             human::AnimeExpressionSystem& expressions,
                             render::AnimeEyeSystem& eyes) {
    motion_ = &motion;
    expressions_ = &expressions;
    eyes_ = &eyes;
}

void AnimationPolish::update(f32 delta_seconds) {
    if (motion_ == nullptr) {
        return;
    }
    motion_->update(delta_seconds);
    if (eyes_ != nullptr && eye_contact_) {
        eyes_->set_look_target({0.0f, 0.1f});
        eyes_->set_gaze(render::EyeGazeMode::LookAtTarget);
    }
    if (transitioning_) {
        transition_timer_ -= delta_seconds;
        if (transition_timer_ <= 0.0f) {
            transitioning_ = false;
            play_idle();
        }
    }
}

void AnimationPolish::play_idle() {
    if (motion_ == nullptr) {
        return;
    }
    motion_->set_body_language(animation::AnimeBodyLanguageId::IdleBreathing);
}

void AnimationPolish::play_sit_transition() {
    if (motion_ == nullptr) {
        return;
    }
    motion_->trigger_reaction("sit");
    motion_->set_body_language(animation::AnimeBodyLanguageId::Neutral);
    transitioning_ = true;
    transition_timer_ = 0.6f;
}

void AnimationPolish::play_stand_transition() {
    if (motion_ == nullptr) {
        return;
    }
    motion_->trigger_reaction("stand");
    play_idle();
    transitioning_ = true;
    transition_timer_ = 0.5f;
}

void AnimationPolish::play_walk_turn() {
    if (motion_ == nullptr) {
        return;
    }
    motion_->set_body_language(animation::AnimeBodyLanguageId::OutfitPreviewWalk);
    transitioning_ = true;
    transition_timer_ = 1.2f;
}

void AnimationPolish::play_mirror_pose() {
    if (motion_ == nullptr) {
        return;
    }
    motion_->set_body_language(animation::AnimeBodyLanguageId::MirrorPose);
}

void AnimationPolish::play_outfit_preview_pose() {
    if (motion_ == nullptr) {
        return;
    }
    motion_->set_body_language(animation::AnimeBodyLanguageId::OutfitPreviewWalk);
}

void AnimationPolish::play_dialogue_reaction(std::string_view mood) {
    if (motion_ == nullptr || expressions_ == nullptr) {
        return;
    }
    motion_->trigger_reaction(mood);
    if (mood == "happy" || mood == "soft_smile") {
        expressions_->set_expression("soft_smile");
    } else if (mood == "shy" || mood == "embarrassed") {
        expressions_->set_expression("blushing");
    } else if (mood == "sad") {
        expressions_->set_expression("shy");
    } else {
        expressions_->set_expression("neutral");
    }
    if (eyes_ != nullptr) {
        eyes_->trigger_blink();
    }
}

void AnimationPolish::trigger_hand_gesture(std::string_view gesture_id) {
    if (motion_ == nullptr) {
        return;
    }
    if (gesture_id == "wave") {
        motion_->trigger_reaction("happy");
    } else if (gesture_id == "shy_wave") {
        motion_->trigger_reaction("shy");
    } else {
        motion_->trigger_reaction(gesture_id);
    }
}

void AnimationPolish::set_eye_contact(bool enabled) {
    eye_contact_ = enabled;
    if (eyes_ != nullptr && enabled) {
        eyes_->set_look_target({0.0f, 0.1f});
        eyes_->set_gaze(render::EyeGazeMode::LookAtTarget);
    }
}

} // namespace eve::polish
