#pragma once

#include <eve/scene/scene_types.hpp>

#include <string>
#include <unordered_map>

namespace eve::scene {

struct MusicCueResult {
    bool playing = false;
    std::string track_id;
    f32 volume = 0.0f;
};

class MusicCueSystem {
public:
    bool load_cues(std::string_view path);
    void register_cue(MusicCueDefinition cue);

    MusicCueResult play(std::string_view cue_id) const;
    MusicCueResult stop(std::string_view cue_id) const;
    [[nodiscard]] const MusicCueDefinition* find_cue(std::string_view cue_id) const;
    [[nodiscard]] std::size_t cue_count() const { return cues_.size(); }

private:
    std::unordered_map<std::string, MusicCueDefinition> cues_;
    mutable std::string active_track_;
};

} // namespace eve::scene
