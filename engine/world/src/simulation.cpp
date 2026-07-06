#include <algorithm>
#include <cmath>
#include <eve/world/simulation.hpp>

namespace eve::world {

void SimulationClock::advance(f32 delta_seconds, f32 time_scale) {
    elapsed_seconds += delta_seconds;
    hour += (delta_seconds * time_scale) / 3600.0f;
    while (hour >= day_length_hours) {
        hour -= day_length_hours;
        ++day_index;
    }
}

TimeOfDay SimulationClock::time_of_day() const {
    if (hour >= 5.0f && hour < 12.0f) {
        return TimeOfDay::Morning;
    }
    if (hour >= 12.0f && hour < 17.0f) {
        return TimeOfDay::Afternoon;
    }
    if (hour >= 17.0f && hour < 21.0f) {
        return TimeOfDay::Evening;
    }
    return TimeOfDay::Night;
}

f32 SimulationClock::normalized_time() const {
    return hour / day_length_hours;
}

void ApartmentSimulation::update(f32 delta_seconds) {
    clock.advance(delta_seconds);
    for (auto& [_, appliance] : appliances) {
        if (appliance.powered && appliance.type == "television") {
            appliance.state = ObjectState::On;
        }
    }
    cleanliness = std::max(0.0f, cleanliness - delta_seconds * 0.0001f);
}

void ApartmentSimulation::set_room_lights(std::string_view room_id, bool enabled) {
    auto it = lighting.find(std::string(room_id));
    if (it != lighting.end()) {
        it->second.enabled = enabled;
    } else {
        lighting[std::string(room_id)] =
            LightingZone{std::string(room_id), enabled ? 1.0f : 0.0f, 4000.0f, enabled};
    }
}

void ApartmentSimulation::set_weather(WeatherType weather) {
    clock.weather = weather;
}

f32 ApartmentSimulation::room_light_level(std::string_view room_id) const {
    const auto it = lighting.find(std::string(room_id));
    if (it == lighting.end() || !it->second.enabled) {
        return 0.2f;
    }
    f32 level = it->second.intensity;
    switch (clock.time_of_day()) {
    case TimeOfDay::Morning:
        level *= 1.0f;
        break;
    case TimeOfDay::Afternoon:
        level *= 1.1f;
        break;
    case TimeOfDay::Evening:
        level *= 0.8f;
        break;
    case TimeOfDay::Night:
        level *= 0.5f;
        break;
    }
    return level;
}

} // namespace eve::world
