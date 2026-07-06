#include <eve/core/filesystem/filesystem.hpp>
#include <eve/world/anime_room_lighting.hpp>

#include <nlohmann/json.hpp>

namespace eve::world {

bool AnimeRoomLightingSystem::load_presets(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            AnimeRoomLightingPreset preset;
            preset.id = entry.value("id", "");
            preset.room_id = entry.value("room_id", "wardrobe");
            const std::string mood_str = entry.value("mood", "cozy_lamp");
            if (mood_str == "morning") {
                preset.mood = AnimeLightingMood::Morning;
            } else if (mood_str == "warm_sunset") {
                preset.mood = AnimeLightingMood::WarmSunset;
            } else if (mood_str == "night") {
                preset.mood = AnimeLightingMood::Night;
            } else if (mood_str == "rainy_day") {
                preset.mood = AnimeLightingMood::RainyDay;
            } else if (mood_str == "neon_city") {
                preset.mood = AnimeLightingMood::NeonCity;
            } else {
                preset.mood = AnimeLightingMood::CozyLamp;
            }
            preset.key_intensity = entry.value("key_intensity", 1.2f);
            preset.window_beam_strength = entry.value("window_beam", 0.0f);
            preset.mirror_reflections = entry.value("mirror_reflections", true);
            register_preset(std::move(preset));
        }
        return !presets_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void AnimeRoomLightingSystem::register_preset(AnimeRoomLightingPreset preset) {
    presets_[preset.id] = std::move(preset);
}

bool AnimeRoomLightingSystem::apply_room_mood(std::string_view room_id, AnimeLightingMood mood) {
    for (const auto& [id, preset] : presets_) {
        (void)id;
        if (preset.room_id == room_id && preset.mood == mood) {
            active_ = preset;
            return true;
        }
    }
    return false;
}

bool AnimeRoomLightingSystem::apply_preset(std::string_view preset_id) {
    const auto it = presets_.find(std::string(preset_id));
    if (it == presets_.end()) {
        return false;
    }
    active_ = it->second;
    return true;
}

std::vector<std::string> AnimeRoomLightingSystem::presets_for_room(std::string_view room_id) const {
    std::vector<std::string> ids;
    for (const auto& [id, preset] : presets_) {
        if (preset.room_id == room_id) {
            ids.push_back(id);
        }
    }
    return ids;
}

} // namespace eve::world
