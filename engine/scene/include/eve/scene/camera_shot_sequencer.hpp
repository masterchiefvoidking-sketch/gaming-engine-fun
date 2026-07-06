#pragma once

#include <eve/render/romance_camera.hpp>
#include <eve/scene/scene_types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::scene {

struct ShotPlaybackState {
    std::string active_shot_id;
    f32 elapsed_seconds = 0.0f;
    f32 duration_seconds = 0.0f;
    bool completed = false;
};

class CameraShotSequencer {
public:
    bool load_shots(std::string_view path);
    void register_shot(CameraShotDefinition shot);

    bool play_shot(std::string_view shot_id, render::RomanceCameraController& camera);
    void update(f32 delta_seconds);
    void reset();

    [[nodiscard]] const CameraShotDefinition* find_shot(std::string_view shot_id) const;
    [[nodiscard]] const ShotPlaybackState& playback() const { return playback_; }
    [[nodiscard]] std::size_t shot_count() const { return shots_.size(); }

private:
    std::unordered_map<std::string, CameraShotDefinition> shots_;
    ShotPlaybackState playback_;
    const CameraShotDefinition* active_shot_ = nullptr;
};

} // namespace eve::scene
