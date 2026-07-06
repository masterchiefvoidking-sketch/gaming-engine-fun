#include <cmath>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/render/romance_camera.hpp>

#include <nlohmann/json.hpp>

namespace eve::render {

void RomanceCameraController::attach(InspectionCamera& camera) {
    camera_ = &camera;
}

void RomanceCameraController::load_presets(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            RomanceCameraPreset preset;
            preset.id = entry.value("id", "");
            preset.label = entry.value("label", preset.id);
            preset.fov_y_degrees = entry.value("fov", 35.0f);
            preset.focal_length_mm = entry.value("focal_length_mm", 50.0f);
            preset.rule_of_thirds_guide = entry.value("rule_of_thirds", false);
            if (entry.contains("position")) {
                preset.position.x = entry["position"].value("x", 0.0f);
                preset.position.y = entry["position"].value("y", 1.5f);
                preset.position.z = entry["position"].value("z", 3.0f);
            }
            if (entry.contains("focus")) {
                preset.focus.x = entry["focus"].value("x", 0.0f);
                preset.focus.y = entry["focus"].value("y", 1.0f);
                preset.focus.z = entry["focus"].value("z", 0.0f);
            }
            if (entry.contains("depth_of_field")) {
                preset.depth_of_field.enabled = entry["depth_of_field"].value("enabled", false);
                preset.depth_of_field.focus_distance =
                    entry["depth_of_field"].value("focus_distance", 2.0f);
                preset.depth_of_field.aperture = entry["depth_of_field"].value("aperture", 2.8f);
            }
            register_preset(std::move(preset));
        }
    } catch (const nlohmann::json::exception&) {
    }
}

void RomanceCameraController::register_preset(RomanceCameraPreset preset) {
    presets_[preset.id] = std::move(preset);
}

bool RomanceCameraController::apply_preset(std::string_view preset_id) {
    const auto it = presets_.find(std::string(preset_id));
    if (it == presets_.end() || camera_ == nullptr) {
        return false;
    }
    active_preset_ = &it->second;
    camera_->apply_preset(preset_id);
    camera_->add_preset({it->second.id, it->second.label, it->second.position, it->second.focus,
                         it->second.fov_y_degrees});
    camera_->apply_preset(it->second.id);
    camera_->depth_of_field_mutable() = it->second.depth_of_field;
    guide_.enabled = it->second.rule_of_thirds_guide;
    if (guide_.enabled) {
        guide_.intersection_point = {0.33f, 0.33f};
    }
    return true;
}

bool RomanceCameraController::apply_romance_preset(RomanceCameraPresetId preset_id) {
    switch (preset_id) {
    case RomanceCameraPresetId::FaceCloseUp:
        return apply_preset("face_closeup");
    case RomanceCameraPresetId::EyeCloseUp:
        return apply_preset("eye_closeup");
    case RomanceCameraPresetId::FullBodyWardrobe:
        return apply_preset("full_body_wardrobe");
    case RomanceCameraPresetId::MirrorView:
        return apply_preset("mirror_view");
    case RomanceCameraPresetId::PhotoPortrait:
        return apply_preset("photo_portrait");
    default:
        return apply_preset("full_body_wardrobe");
    }
}

void RomanceCameraController::enable_rule_of_thirds(bool enabled) {
    guide_.enabled = enabled;
}

void RomanceCameraController::set_focal_length(f32 focal_length_mm) {
    if (camera_ == nullptr) {
        return;
    }
    const f32 fov = 2.0f * std::atan(24.0f / (2.0f * focal_length_mm)) * 57.2958f;
    camera_->set_fov(fov);
}

void RomanceCameraController::transition_to_preset(std::string_view preset_id, f32 duration_seconds) {
    const auto it = presets_.find(std::string(preset_id));
    if (it == presets_.end() || camera_ == nullptr) {
        return;
    }
    active_preset_ = &it->second;
    camera_->transition_to(it->second.position, it->second.focus, duration_seconds);
    camera_->depth_of_field_mutable() = it->second.depth_of_field;
}

} // namespace eve::render
