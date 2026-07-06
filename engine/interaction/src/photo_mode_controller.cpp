#include <eve/interaction/photo_mode_controller.hpp>

namespace eve::interaction {

bool PhotoModeController::initialize(render::InspectionCamera& camera,
                                     human::AnimeVisualPipeline& pipeline) {
    camera_ = &camera;
    pipeline_ = &pipeline;
    camera.set_mode(render::CameraMode::Photo);
    settings_.lighting_preset = "studio_soft";
    settings_.expression_preset = "soft_smile";
    return true;
}

void PhotoModeController::enable(bool enabled) {
    enabled_ = enabled;
    if (camera_ != nullptr) {
        camera_->set_mode(enabled ? render::CameraMode::Photo : render::CameraMode::Orbit);
    }
}

void PhotoModeController::set_lighting_preset(std::string_view preset_id) {
    settings_.lighting_preset = std::string(preset_id);
}

void PhotoModeController::set_expression(std::string_view expression_id) {
    settings_.expression_preset = std::string(expression_id);
    if (pipeline_ != nullptr) {
        pipeline_->set_expression(expression_id);
    }
}

void PhotoModeController::set_outfit(std::string_view outfit_id) {
    if (pipeline_ != nullptr) {
        pipeline_->change_outfit(outfit_id);
    }
}

void PhotoModeController::set_hair(std::string_view hair_id) {
    if (pipeline_ != nullptr) {
        pipeline_->change_hair(hair_id);
    }
}

void PhotoModeController::set_background_blur(bool enabled) {
    settings_.background_blur = enabled;
    if (camera_ != nullptr) {
        camera_->depth_of_field_mutable().enabled = enabled;
    }
}

void PhotoModeController::set_hide_ui(bool hidden) {
    hide_ui_ = hidden;
}

bool PhotoModeController::capture(std::string_view path) {
    if (pipeline_ == nullptr) {
        return false;
    }
    return pipeline_->capture_screenshot(path);
}

} // namespace eve::interaction
