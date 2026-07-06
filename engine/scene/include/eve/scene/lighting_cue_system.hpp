#pragma once

#include <eve/scene/scene_types.hpp>
#include <eve/world/anime_room_lighting.hpp>

#include <string>
#include <unordered_map>

namespace eve::scene {

struct LightingCueResult {
    bool applied = false;
    std::string preset_id;
    std::string room_id;
};

class LightingCueSystem {
public:
    bool load_presets(std::string_view path);
    void register_cue(LightingCueDefinition cue);

    LightingCueResult apply(std::string_view cue_id, world::AnimeRoomLightingSystem& lighting) const;
    [[nodiscard]] std::size_t cue_count() const { return cues_.size(); }

private:
    std::unordered_map<std::string, LightingCueDefinition> cues_;
};

} // namespace eve::scene
