#include <eve/scene/scene_types.hpp>

namespace eve::scene {

const char* to_string(SceneState state) {
    switch (state) {
    case SceneState::Loading:
        return "loading";
    case SceneState::Entering:
        return "entering";
    case SceneState::Playing:
        return "playing";
    case SceneState::WaitingChoice:
        return "waiting_choice";
    case SceneState::Transitioning:
        return "transitioning";
    case SceneState::Completed:
        return "completed";
    case SceneState::Failed:
        return "failed";
    case SceneState::Cooldown:
        return "cooldown";
    default:
        return "idle";
    }
}

const char* to_string(EaseCurve curve) {
    switch (curve) {
    case EaseCurve::EaseIn:
        return "ease_in";
    case EaseCurve::EaseOut:
        return "ease_out";
    case EaseCurve::EaseInOut:
        return "ease_in_out";
    case EaseCurve::SmoothStep:
        return "smooth_step";
    default:
        return "linear";
    }
}

} // namespace eve::scene
