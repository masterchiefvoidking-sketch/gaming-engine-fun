#include <eve/interaction/character_inspection_mode.hpp>

#include <eve/core/logging/logger.hpp>

namespace eve::interaction {

bool CharacterInspectionMode::initialize(std::string_view data_root) {
    if (!pipeline_.initialize(data_root)) {
        return false;
    }
    if (!photo_mode_.initialize(camera_, pipeline_)) {
        return false;
    }
    body_registry_.load_from_directory(std::string(data_root) + "/interactions");
    human::load_body_volumes_from_file(std::string(data_root) + "/characters/default/body_definition.json",
                                       body_volumes_);
    camera_.set_mode(render::CameraMode::Orbit);
    preview_.outfit_id = "mira_outfit_01";
    preview_.hair_id = "mira_hair_bun";
    preview_.expression_id = "soft_smile";
    pipeline_.change_outfit(preview_.outfit_id);
    pipeline_.change_hair(preview_.hair_id);
    pipeline_.set_expression(preview_.expression_id);
    initialized_ = true;
    EVE_LOG(Info, "CharacterInspection", "Inspection mode ready");
    return true;
}

void CharacterInspectionMode::shutdown() {
    if (initialized_) {
        pipeline_.shutdown();
        initialized_ = false;
    }
}

void CharacterInspectionMode::update(f32 delta_seconds, InputController& input) {
    if (!initialized_) {
        return;
    }
    input.begin_frame();
    camera_bridge_.apply(camera_, delta_seconds, input.frame());
    pipeline_.update(delta_seconds);
}

bool CharacterInspectionMode::focus_body_region(human::BodyRegion region) {
    for (const human::BodyRegionVolume& volume : body_volumes_) {
        if (volume.region != region) {
            continue;
        }
        preview_.focused_region = region;
        camera_.focus_body_region(volume.center, volume.focus_offset);
        return true;
    }
    return false;
}

bool CharacterInspectionMode::preview_outfit(std::string_view outfit_id) {
    if (!pipeline_.change_outfit(outfit_id)) {
        return false;
    }
    preview_.outfit_id = std::string(outfit_id);
    return true;
}

bool CharacterInspectionMode::preview_hair(std::string_view hair_id) {
    if (!pipeline_.change_hair(hair_id)) {
        return false;
    }
    preview_.hair_id = std::string(hair_id);
    return true;
}

bool CharacterInspectionMode::preview_expression(std::string_view expression_id) {
    if (!pipeline_.set_expression(expression_id)) {
        return false;
    }
    preview_.expression_id = std::string(expression_id);
    return true;
}

bool CharacterInspectionMode::preview_pose(std::string_view pose_id) {
    preview_.pose_id = std::string(pose_id);
    pipeline_.apply_camera_preset(pose_id);
    return true;
}

bool CharacterInspectionMode::apply_lighting_preset(std::string_view preset_id) {
    preview_.lighting_preset = std::string(preset_id);
    photo_mode_.set_lighting_preset(preset_id);
    return true;
}

bool CharacterInspectionMode::capture_screenshot(std::string_view path) {
    return pipeline_.capture_screenshot(path);
}

human::BodyRegionTarget CharacterInspectionMode::raycast_body_region(
    const math::Vec3& origin, const math::Vec3& direction) const {
    return body_interactions_.query_region(body_volumes_, origin, direction);
}

} // namespace eve::interaction
