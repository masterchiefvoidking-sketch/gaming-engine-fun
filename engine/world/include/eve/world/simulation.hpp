#pragma once

#include <eve/world/apartment_types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::world {

struct LightingZone {
    std::string room_id;
    f32 intensity = 1.0f;
    f32 color_temperature = 4000.0f;
    bool enabled = true;
};

struct AmbientAudioSource {
    std::string id;
    std::string sound_id;
    f32 volume = 0.5f;
    bool playing = false;
    std::string room_id;
};

struct ApplianceState {
    std::string id;
    std::string type;
    bool powered = false;
    std::string current_media;
    f32 volume = 0.5f;
    ObjectState state = ObjectState::Off;
};

struct SimulationClock {
    f32 hour = 8.0f;
    f32 day_length_hours = 24.0f;
    u32 day_index = 0;
    WeatherType weather = WeatherType::Clear;
    f32 elapsed_seconds = 0.0f;

    void advance(f32 delta_seconds, f32 time_scale = 60.0f);
    [[nodiscard]] TimeOfDay time_of_day() const;
    [[nodiscard]] f32 normalized_time() const;
};

struct ApartmentSimulation {
    SimulationClock clock;
    std::unordered_map<std::string, LightingZone> lighting;
    std::vector<AmbientAudioSource> ambient_audio;
    std::unordered_map<std::string, ApplianceState> appliances;
    f32 cleanliness = 1.0f;
    f32 pantry_freshness = 1.0f;

    void update(f32 delta_seconds);
    void set_room_lights(std::string_view room_id, bool enabled);
    void set_weather(WeatherType weather);
    [[nodiscard]] f32 room_light_level(std::string_view room_id) const;
};

} // namespace eve::world
