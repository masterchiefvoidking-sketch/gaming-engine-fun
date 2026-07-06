#include <algorithm>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/core/logging/logger.hpp>
#include <eve/render/anime_human_material.hpp>

#include <nlohmann/json.hpp>

namespace eve::render {

namespace {

ToonRampStop parse_ramp_stop(const nlohmann::json& json) {
    ToonRampStop stop;
    stop.threshold = json.value("threshold", 0.0f);
    if (json.contains("color")) {
        stop.color.x = json["color"].value("r", 1.0f);
        stop.color.y = json["color"].value("g", 1.0f);
        stop.color.z = json["color"].value("b", 1.0f);
    }
    return stop;
}

math::Vec3 parse_color(const nlohmann::json& json, const math::Vec3& fallback) {
    if (!json.is_object()) {
        return fallback;
    }
    return {json.value("r", fallback.x), json.value("g", fallback.y), json.value("b", fallback.z)};
}

} // namespace

bool AnimeHumanMaterial::initialize() {
    toon_ramp_ = {
        {0.0f, {0.75f, 0.62f, 0.58f}},
        {0.35f, {0.92f, 0.78f, 0.72f}},
        {0.65f, {0.98f, 0.88f, 0.84f}},
        {1.0f, {1.0f, 0.95f, 0.92f}},
    };
    ready_ = true;
    EVE_LOG(Info, "AnimeRender", "Anime human material initialized");
    return true;
}

void AnimeHumanMaterial::shutdown() {
    ready_ = false;
}

void AnimeHumanMaterial::load_presets(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            AnimeMaterialPreset preset;
            preset.id = entry.value("id", "");
            preset.label = entry.value("label", preset.id);
            if (entry.contains("skin")) {
                const auto& skin = entry["skin"];
                preset.skin.base_tone = parse_color(skin["base_tone"], preset.skin.base_tone);
                preset.skin.shadow_tone = parse_color(skin["shadow_tone"], preset.skin.shadow_tone);
                preset.skin.blush_intensity = skin.value("blush_intensity", 0.0f);
                preset.skin.rim_light_intensity = skin.value("rim_light_intensity", 0.45f);
            }
            if (entry.contains("makeup")) {
                preset.makeup.lip_tint_intensity = entry["makeup"].value("lip_tint", 0.35f);
            }
            if (entry.contains("outline")) {
                preset.outline.thickness = entry["outline"].value("thickness", 1.2f);
            }
            if (entry.contains("toon_ramp")) {
                for (const auto& stop : entry["toon_ramp"]) {
                    preset.toon_ramp.push_back(parse_ramp_stop(stop));
                }
            }
            register_preset(std::move(preset));
        }
    } catch (const nlohmann::json::exception&) {
    }
}

void AnimeHumanMaterial::register_preset(AnimeMaterialPreset preset) {
    presets_[preset.id] = std::move(preset);
}

bool AnimeHumanMaterial::apply_preset(std::string_view preset_id) {
    const AnimeMaterialPreset* preset = find_preset(preset_id);
    if (preset == nullptr) {
        return false;
    }
    skin_ = preset->skin;
    makeup_ = preset->makeup;
    outline_ = preset->outline;
    if (!preset->toon_ramp.empty()) {
        toon_ramp_ = preset->toon_ramp;
    }
    return true;
}

void AnimeHumanMaterial::set_skin_params(const AnimeSkinParams& skin) {
    skin_ = skin;
}

void AnimeHumanMaterial::set_makeup_params(const AnimeMakeupParams& makeup) {
    makeup_ = makeup;
}

void AnimeHumanMaterial::set_outline_params(const AnimeOutlineParams& outline) {
    outline_ = outline;
}

void AnimeHumanMaterial::set_blush(f32 intensity) {
    skin_.blush_intensity = std::clamp(intensity, 0.0f, 1.0f);
}

void AnimeHumanMaterial::set_mood_tint(const math::Vec3& tint) {
    skin_.mood_tint = tint;
}

void AnimeHumanMaterial::set_wetness(f32 shine) {
    skin_.wetness_shine = std::clamp(shine, 0.0f, 1.0f);
}

const AnimeMaterialPreset* AnimeHumanMaterial::find_preset(std::string_view id) const {
    const auto it = presets_.find(std::string(id));
    if (it == presets_.end()) {
        return nullptr;
    }
    return &it->second;
}

f32 evaluate_toon_ramp(f32 ndotl, const std::vector<ToonRampStop>& ramp) {
    if (ramp.empty()) {
        return ndotl;
    }
    ndotl = std::clamp(ndotl, 0.0f, 1.0f);
    for (std::size_t i = 1; i < ramp.size(); ++i) {
        if (ndotl <= ramp[i].threshold) {
            const f32 t = (ndotl - ramp[i - 1].threshold) /
                          std::max(ramp[i].threshold - ramp[i - 1].threshold, 0.001f);
            const f32 band = t < 0.5f ? 0.0f : 1.0f;
            (void)band;
            return ramp[i - 1].color.x * (1.0f - t) + ramp[i].color.x * t;
        }
    }
    return ramp.back().color.x;
}

} // namespace eve::render
