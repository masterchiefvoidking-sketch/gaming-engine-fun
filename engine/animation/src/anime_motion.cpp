#include <cmath>
#include <eve/animation/anime_motion.hpp>

namespace eve::animation {

void AnimeMotionController::set_body_language(AnimeBodyLanguageId language) {
    state_.body_language = language;
    switch (language) {
    case AnimeBodyLanguageId::Shy:
        state_.head_tilt = -0.08f;
        state_.shoulder_offset = 0.05f;
        state_.active_clip = "anim_shy_idle";
        break;
    case AnimeBodyLanguageId::Confident:
        state_.head_tilt = 0.05f;
        state_.shoulder_offset = -0.03f;
        state_.active_clip = "anim_confident_idle";
        break;
    case AnimeBodyLanguageId::Happy:
        state_.active_clip = "anim_happy_idle";
        break;
    case AnimeBodyLanguageId::Embarrassed:
        state_.head_tilt = -0.12f;
        state_.active_clip = "anim_embarrassed";
        break;
    case AnimeBodyLanguageId::MirrorPose:
        state_.active_clip = "anim_mirror_pose";
        break;
    case AnimeBodyLanguageId::OutfitPreviewWalk:
        state_.active_clip = "anim_walk_preview";
        break;
    default:
        state_.head_tilt = 0.0f;
        state_.shoulder_offset = 0.0f;
        state_.active_clip = "anim_idle";
        break;
    }
}

void AnimeMotionController::trigger_reaction(std::string_view reaction_id) {
    if (reaction_id == "happy") {
        set_body_language(AnimeBodyLanguageId::Happy);
    } else if (reaction_id == "embarrassed") {
        set_body_language(AnimeBodyLanguageId::Embarrassed);
    } else if (reaction_id == "shy") {
        set_body_language(AnimeBodyLanguageId::Shy);
    }
}

void AnimeMotionController::set_eye_tracking(bool enabled) {
    eye_tracking_ = enabled;
}

void AnimeMotionController::update(f32 delta_seconds) {
    elapsed_ += delta_seconds;
    state_.breathing_offset = std::sin(elapsed_ * 2.5f) * 0.015f;
    state_.hair_sway = std::sin(elapsed_ * 1.8f) * 0.03f;
    state_.cloth_sway = std::sin(elapsed_ * 1.4f) * 0.02f;
    (void)eye_tracking_;
}

} // namespace eve::animation
