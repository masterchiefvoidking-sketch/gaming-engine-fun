#include <algorithm>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/human/anime_expression_system.hpp>

#include <nlohmann/json.hpp>

namespace eve::human {

const char* to_string(AnimeExpressionId expression) {
    switch (expression) {
    case AnimeExpressionId::Smile:
        return "smile";
    case AnimeExpressionId::SoftSmile:
        return "soft_smile";
    case AnimeExpressionId::Laugh:
        return "laugh";
    case AnimeExpressionId::Embarrassed:
        return "embarrassed";
    case AnimeExpressionId::Blushing:
        return "blushing";
    case AnimeExpressionId::Shy:
        return "shy";
    case AnimeExpressionId::Annoyed:
        return "annoyed";
    case AnimeExpressionId::Sad:
        return "sad";
    case AnimeExpressionId::Surprised:
        return "surprised";
    case AnimeExpressionId::Flirty:
        return "flirty";
    case AnimeExpressionId::Sleepy:
        return "sleepy";
    case AnimeExpressionId::Curious:
        return "curious";
    case AnimeExpressionId::Pouting:
        return "pouting";
    case AnimeExpressionId::Confident:
        return "confident";
    case AnimeExpressionId::RomanticGaze:
        return "romantic_gaze";
    default:
        return "neutral";
    }
}

AnimeExpressionId expression_from_string(std::string_view value) {
    if (value == "smile") return AnimeExpressionId::Smile;
    if (value == "soft_smile") return AnimeExpressionId::SoftSmile;
    if (value == "laugh") return AnimeExpressionId::Laugh;
    if (value == "embarrassed") return AnimeExpressionId::Embarrassed;
    if (value == "blushing") return AnimeExpressionId::Blushing;
    if (value == "shy") return AnimeExpressionId::Shy;
    if (value == "annoyed") return AnimeExpressionId::Annoyed;
    if (value == "sad") return AnimeExpressionId::Sad;
    if (value == "surprised") return AnimeExpressionId::Surprised;
    if (value == "flirty") return AnimeExpressionId::Flirty;
    if (value == "sleepy") return AnimeExpressionId::Sleepy;
    if (value == "curious") return AnimeExpressionId::Curious;
    if (value == "pouting") return AnimeExpressionId::Pouting;
    if (value == "confident") return AnimeExpressionId::Confident;
    if (value == "romantic_gaze") return AnimeExpressionId::RomanticGaze;
    return AnimeExpressionId::Neutral;
}

bool AnimeExpressionSystem::load_presets(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            AnimeExpressionPreset preset;
            preset.id = entry.value("id", "");
            preset.expression = expression_from_string(entry.value("expression", "neutral"));
            if (entry.contains("channels")) {
                const auto& ch = entry["channels"];
                preset.channels.brow_raise = ch.value("brow_raise", 0.0f);
                preset.channels.brow_lower = ch.value("brow_lower", 0.0f);
                preset.channels.eye_open = ch.value("eye_open", 1.0f);
                preset.channels.mouth_smile = ch.value("mouth_smile", 0.0f);
                preset.channels.blush = ch.value("blush", 0.0f);
                preset.channels.head_tilt = ch.value("head_tilt", 0.0f);
                preset.channels.eye_squint = ch.value("eye_squint", 0.0f);
                preset.channels.mouth_open = ch.value("mouth_open", 0.0f);
                preset.channels.cheek_raise = ch.value("cheek_raise", 0.0f);
                preset.channels.shoulder_pose = ch.value("shoulder_pose", 0.0f);
                preset.channels.voice_reaction_hook = ch.value("voice_hook", "");
            }
            register_preset(std::move(preset));
        }
        return !presets_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void AnimeExpressionSystem::register_preset(AnimeExpressionPreset preset) {
    presets_[preset.id] = std::move(preset);
}

bool AnimeExpressionSystem::set_expression(std::string_view expression_id, f32 blend_weight) {
    const AnimeExpressionPreset* preset = find(expression_id);
    if (preset == nullptr) {
        return false;
    }
    target_ = blend_toward(preset->channels, blend_weight);
    return true;
}

bool AnimeExpressionSystem::set_expression(AnimeExpressionId expression, f32 blend_weight) {
    return set_expression(to_string(expression), blend_weight);
}

void AnimeExpressionSystem::update(f32 delta_seconds) {
    const f32 t = std::clamp(delta_seconds * blend_speed_, 0.0f, 1.0f);
    blended_.brow_raise += (target_.brow_raise - blended_.brow_raise) * t;
    blended_.brow_lower += (target_.brow_lower - blended_.brow_lower) * t;
    blended_.eye_open += (target_.eye_open - blended_.eye_open) * t;
    blended_.eye_squint += (target_.eye_squint - blended_.eye_squint) * t;
    blended_.mouth_smile += (target_.mouth_smile - blended_.mouth_smile) * t;
    blended_.mouth_open += (target_.mouth_open - blended_.mouth_open) * t;
    blended_.cheek_raise += (target_.cheek_raise - blended_.cheek_raise) * t;
    blended_.blush += (target_.blush - blended_.blush) * t;
    blended_.head_tilt += (target_.head_tilt - blended_.head_tilt) * t;
    blended_.shoulder_pose += (target_.shoulder_pose - blended_.shoulder_pose) * t;
}

const AnimeExpressionPreset* AnimeExpressionSystem::find(std::string_view id) const {
    const auto it = presets_.find(std::string(id));
    if (it == presets_.end()) {
        return nullptr;
    }
    return &it->second;
}

AnimeExpressionChannels AnimeExpressionSystem::blend_toward(const AnimeExpressionChannels& target,
                                                            f32 weight) const {
    AnimeExpressionChannels result = target;
    result.brow_raise *= weight;
    result.brow_lower *= weight;
    result.eye_open = 1.0f + (target.eye_open - 1.0f) * weight;
    result.mouth_smile *= weight;
    result.blush *= weight;
    result.head_tilt *= weight;
    return result;
}

} // namespace eve::human
