#include <eve/scene/music_cue_system.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::scene {

bool MusicCueSystem::load_cues(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            MusicCueDefinition cue;
            cue.id = entry.value("id", "");
            cue.label = entry.value("label", "");
            cue.track_id = entry.value("track", "");
            cue.volume = entry.value("volume", 0.7f);
            cue.fade_in_seconds = entry.value("fade_in", 1.0f);
            cue.fade_out_seconds = entry.value("fade_out", 1.0f);
            cue.loop = entry.value("loop", true);
            register_cue(std::move(cue));
        }
        return !cues_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void MusicCueSystem::register_cue(MusicCueDefinition cue) {
    cues_[cue.id] = std::move(cue);
}

MusicCueResult MusicCueSystem::play(std::string_view cue_id) const {
    MusicCueResult result;
    const MusicCueDefinition* cue = find_cue(cue_id);
    if (cue == nullptr) {
        return result;
    }
    active_track_ = cue->track_id;
    result.playing = true;
    result.track_id = cue->track_id;
    result.volume = cue->volume;
    return result;
}

MusicCueResult MusicCueSystem::stop(std::string_view cue_id) const {
    MusicCueResult result;
    const MusicCueDefinition* cue = find_cue(cue_id);
    if (cue == nullptr) {
        return result;
    }
    if (active_track_ == cue->track_id) {
        active_track_.clear();
    }
    result.playing = false;
    result.track_id = cue->track_id;
    return result;
}

const MusicCueDefinition* MusicCueSystem::find_cue(std::string_view cue_id) const {
    const auto it = cues_.find(std::string(cue_id));
    if (it == cues_.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace eve::scene
