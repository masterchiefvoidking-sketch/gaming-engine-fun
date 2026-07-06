#include <eve/scene/camera_shot_sequencer.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::scene {

namespace {

EaseCurve parse_ease(const std::string& value) {
    if (value == "ease_in") return EaseCurve::EaseIn;
    if (value == "ease_out") return EaseCurve::EaseOut;
    if (value == "ease_in_out") return EaseCurve::EaseInOut;
    if (value == "smooth_step") return EaseCurve::SmoothStep;
    return EaseCurve::Linear;
}

} // namespace

bool CameraShotSequencer::load_shots(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            CameraShotDefinition shot;
            shot.id = entry.value("id", "");
            shot.label = entry.value("label", "");
            shot.fov_y_degrees = entry.value("fov", 35.0f);
            shot.focal_length_mm = entry.value("focal_length", 50.0f);
            shot.depth_of_field_strength = entry.value("dof", 0.0f);
            shot.duration_seconds = entry.value("duration", 2.0f);
            shot.ease = parse_ease(entry.value("ease", "ease_in_out"));
            shot.lighting_override_id = entry.value("lighting", "");
            shot.animation_cue_id = entry.value("animation_cue", "");
            if (entry.contains("position")) {
                shot.position.x = entry["position"].value("x", 0.0f);
                shot.position.y = entry["position"].value("y", 0.0f);
                shot.position.z = entry["position"].value("z", 0.0f);
            }
            if (entry.contains("target")) {
                shot.target.x = entry["target"].value("x", 0.0f);
                shot.target.y = entry["target"].value("y", 0.0f);
                shot.target.z = entry["target"].value("z", 0.0f);
            }
            if (entry.contains("focus")) {
                shot.focus_target.x = entry["focus"].value("x", 0.0f);
                shot.focus_target.y = entry["focus"].value("y", 0.0f);
                shot.focus_target.z = entry["focus"].value("z", 0.0f);
            }
            register_shot(std::move(shot));
        }
        return !shots_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void CameraShotSequencer::register_shot(CameraShotDefinition shot) {
    shots_[shot.id] = std::move(shot);
}

bool CameraShotSequencer::play_shot(std::string_view shot_id,
                                    render::RomanceCameraController& camera) {
    const auto it = shots_.find(std::string(shot_id));
    if (it == shots_.end()) {
        return false;
    }
    active_shot_ = &it->second;
    playback_.active_shot_id = it->second.id;
    playback_.elapsed_seconds = 0.0f;
    playback_.duration_seconds = it->second.duration_seconds;
    playback_.completed = false;

    render::RomanceCameraPreset preset;
    preset.id = it->second.id;
    preset.label = it->second.label;
    preset.position = it->second.position;
    preset.focus = it->second.target;
    preset.fov_y_degrees = it->second.fov_y_degrees;
    preset.focal_length_mm = it->second.focal_length_mm;
    preset.depth_of_field.enabled = it->second.depth_of_field_strength > 0.01f;
    preset.depth_of_field.bokeh_radius = it->second.depth_of_field_strength * 0.05f;
    camera.register_preset(preset);
    camera.apply_preset(it->second.id);
    camera.transition_to_preset(it->second.id, it->second.duration_seconds);
    return true;
}

void CameraShotSequencer::update(f32 delta_seconds) {
    if (active_shot_ == nullptr || playback_.completed) {
        return;
    }
    playback_.elapsed_seconds += delta_seconds;
    if (playback_.elapsed_seconds >= playback_.duration_seconds) {
        playback_.completed = true;
    }
}

void CameraShotSequencer::reset() {
    playback_ = {};
    active_shot_ = nullptr;
}

const CameraShotDefinition* CameraShotSequencer::find_shot(std::string_view shot_id) const {
    const auto it = shots_.find(std::string(shot_id));
    if (it == shots_.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace eve::scene
