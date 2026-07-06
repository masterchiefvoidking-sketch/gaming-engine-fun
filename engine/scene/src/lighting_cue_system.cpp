#include <eve/scene/lighting_cue_system.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::scene {

bool LightingCueSystem::load_presets(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            LightingCueDefinition cue;
            cue.id = entry.value("id", "");
            cue.label = entry.value("label", "");
            cue.room_id = entry.value("room", "");
            cue.preset_id = entry.value("preset", "");
            cue.transition_seconds = entry.value("transition", 1.0f);
            register_cue(std::move(cue));
        }
        return !cues_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void LightingCueSystem::register_cue(LightingCueDefinition cue) {
    cues_[cue.id] = std::move(cue);
}

LightingCueResult LightingCueSystem::apply(std::string_view cue_id,
                                           world::AnimeRoomLightingSystem& lighting) const {
    LightingCueResult result;
    const auto it = cues_.find(std::string(cue_id));
    if (it == cues_.end()) {
        return result;
    }
    if (!it->second.preset_id.empty()) {
        lighting.apply_preset(it->second.preset_id);
    } else if (!it->second.room_id.empty()) {
        lighting.apply_room_mood(it->second.room_id, world::AnimeLightingMood::CozyLamp);
    }
    result.applied = true;
    result.preset_id = it->second.preset_id;
    result.room_id = it->second.room_id;
    return result;
}

} // namespace eve::scene
