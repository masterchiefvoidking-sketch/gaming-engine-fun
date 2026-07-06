#pragma once

#include <eve/world/anime_room_lighting.hpp>

#include <string>

namespace eve::polish {

enum class TimeOfDayPreset : u8 {
    Morning = 0,
    Afternoon,
    Sunset,
    RainyEvening,
    Night,
};

struct AmbientAudioCue {
    std::string id;
    std::string path;
    f32 volume = 0.5f;
    bool loop = true;
};

struct ApartmentAtmosphereState {
    TimeOfDayPreset time_preset = TimeOfDayPreset::Sunset;
    std::string active_lighting_id;
    f32 lamp_intensity = 0.7f;
    f32 tv_glow = 0.0f;
    bool mirror_reflections = true;
    bool window_reflections = true;
    std::string ambient_audio_id;
    std::string footstep_surface = "wood";
};

class ApartmentAtmosphere {
public:
    bool load(std::string_view data_root);
    bool apply_time_preset(world::AnimeRoomLightingSystem& lighting, TimeOfDayPreset preset);
    bool apply_room(std::string_view room_id, world::AnimeRoomLightingSystem& lighting);
    void set_lamp_intensity(f32 intensity);
    void set_tv_glow(f32 intensity);
    void trigger_object_sound(std::string_view sound_id);
    void set_footstep_surface(std::string_view surface);

    [[nodiscard]] const ApartmentAtmosphereState& state() const { return state_; }

private:
    ApartmentAtmosphereState state_;
    std::string data_root_;
};

} // namespace eve::polish
