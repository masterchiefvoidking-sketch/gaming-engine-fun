#include <algorithm>
#include <cmath>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/render/anime_eye_system.hpp>

#include <nlohmann/json.hpp>

namespace eve::render {

bool AnimeEyeSystem::initialize() {
    active_preset_.id = "default_sparkle";
    active_preset_.highlights = {
        {{0.22f, 0.28f}, 0.18f, 1.0f, {1.0f, 1.0f, 1.0f}},
        {{0.12f, 0.15f}, 0.08f, 0.6f, {0.9f, 0.95f, 1.0f}},
        {{-0.08f, 0.1f}, 0.05f, 0.35f, {1.0f, 1.0f, 1.0f}},
    };
    return true;
}

void AnimeEyeSystem::load_presets(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            AnimeEyePreset preset;
            preset.id = entry.value("id", "");
            preset.label = entry.value("label", preset.id);
            preset.pupil_scale = entry.value("pupil_scale", 0.55f);
            preset.iris_scale = entry.value("iris_scale", 1.15f);
            preset.gloss_intensity = entry.value("gloss_intensity", 0.85f);
            if (entry.contains("iris_color")) {
                preset.iris_color.x = entry["iris_color"].value("r", 0.35f);
                preset.iris_color.y = entry["iris_color"].value("g", 0.55f);
                preset.iris_color.z = entry["iris_color"].value("b", 0.75f);
            }
            if (entry.contains("highlights")) {
                for (const auto& hl : entry["highlights"]) {
                    EyeHighlightLayer layer;
                    layer.offset.x = hl.value("offset_x", 0.2f);
                    layer.offset.y = hl.value("offset_y", 0.25f);
                    layer.scale = hl.value("scale", 0.15f);
                    layer.intensity = hl.value("intensity", 0.9f);
                    preset.highlights.push_back(layer);
                }
            }
            register_preset(std::move(preset));
        }
    } catch (const nlohmann::json::exception&) {
    }
}

void AnimeEyeSystem::register_preset(AnimeEyePreset preset) {
    presets_[preset.id] = std::move(preset);
}

bool AnimeEyeSystem::apply_preset(std::string_view preset_id) {
    const auto it = presets_.find(std::string(preset_id));
    if (it == presets_.end()) {
        return false;
    }
    active_preset_ = it->second;
    return true;
}

void AnimeEyeSystem::set_gaze(EyeGazeMode gaze) {
    state_.gaze = gaze;
}

void AnimeEyeSystem::set_emotion(EyeEmotionShape emotion) {
    state_.emotion = emotion;
}

void AnimeEyeSystem::set_look_target(const math::Vec2& target) {
    state_.look_target = target;
    state_.gaze = EyeGazeMode::LookAtTarget;
}

void AnimeEyeSystem::trigger_blink() {
    blinking_ = true;
    blink_timer_ = 0.0f;
}

void AnimeEyeSystem::trigger_wink(bool left_eye) {
    if (left_eye) {
        state_.wink_left = 1.0f;
    } else {
        state_.wink_right = 1.0f;
    }
}

void AnimeEyeSystem::update(f32 delta_seconds) {
    if (state_.gaze == EyeGazeMode::Direct || state_.gaze == EyeGazeMode::LookAtTarget) {
        state_.eye_contact_timer += delta_seconds;
    } else {
        state_.eye_contact_timer = 0.0f;
    }

    if (blinking_) {
        blink_timer_ += delta_seconds;
        const f32 duration = 0.15f;
        if (blink_timer_ < duration * 0.5f) {
            state_.blink_amount = blink_timer_ / (duration * 0.5f);
        } else if (blink_timer_ < duration) {
            state_.blink_amount = 1.0f - (blink_timer_ - duration * 0.5f) / (duration * 0.5f);
        } else {
            state_.blink_amount = 0.0f;
            blinking_ = false;
            next_blink_ = 2.5f + std::fmod(state_.eye_contact_timer, 3.0f);
        }
    } else {
        next_blink_ -= delta_seconds;
        if (next_blink_ <= 0.0f) {
            trigger_blink();
        }
    }

    state_.wink_left = std::max(0.0f, state_.wink_left - delta_seconds * 4.0f);
    state_.wink_right = std::max(0.0f, state_.wink_right - delta_seconds * 4.0f);

    switch (state_.emotion) {
    case EyeEmotionShape::Shy:
        state_.tear_gloss = 0.15f;
        break;
    case EyeEmotionShape::Sad:
        state_.tear_gloss = 0.4f;
        break;
    default:
        state_.tear_gloss = 0.0f;
        break;
    }
}

f32 AnimeEyeSystem::pupil_scale() const {
    f32 scale = active_preset_.pupil_scale;
    switch (state_.emotion) {
    case EyeEmotionShape::Surprised:
        return scale * 0.75f;
    case EyeEmotionShape::Happy:
    case EyeEmotionShape::Flirty:
        return scale * 1.05f;
    case EyeEmotionShape::Sleepy:
        return scale * 1.15f;
    default:
        return scale;
    }
}

math::Vec2 AnimeEyeSystem::gaze_offset() const {
    switch (state_.gaze) {
    case EyeGazeMode::ShyGlance:
        return {-0.08f, -0.05f};
    case EyeGazeMode::SideGlance:
        return {0.12f, 0.0f};
    case EyeGazeMode::LookAway:
        return {-0.15f, 0.08f};
    case EyeGazeMode::LookAtTarget:
        return state_.look_target * 0.1f;
    default:
        return {0.0f, 0.0f};
    }
}

} // namespace eve::render
