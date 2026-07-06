#include <eve/ai/scene_interruption.hpp>

namespace eve::ai {

void SceneInterruptionHandler::begin_scene(ActiveScene scene) {
    active_ = std::move(scene);
}

InterruptionResult SceneInterruptionHandler::request_interrupt(ActiveScene new_scene) {
    InterruptionResult result;
    if (!active_.has_value()) {
        active_ = std::move(new_scene);
        result.interrupted = true;
        result.message = "Scene started";
        return result;
    }
    if (!active_->interruptible) {
        result.message = "Current scene cannot be interrupted";
        return result;
    }
    if (static_cast<u8>(new_scene.priority) <= static_cast<u8>(active_->priority)) {
        result.message = "Insufficient priority to interrupt";
        return result;
    }
    suspended_ = *active_;
    result.resumed_scene_id = active_->id;
    active_ = std::move(new_scene);
    result.interrupted = true;
    result.message = "Scene interrupted";
    return result;
}

void SceneInterruptionHandler::end_scene() {
    if (suspended_.has_value()) {
        active_ = std::move(suspended_);
        suspended_.reset();
    } else {
        active_.reset();
    }
}

const ActiveScene* SceneInterruptionHandler::active_scene() const {
    if (!active_.has_value()) {
        return nullptr;
    }
    return &*active_;
}

} // namespace eve::ai
