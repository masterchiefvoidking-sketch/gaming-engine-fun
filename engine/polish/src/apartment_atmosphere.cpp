#include <eve/polish/apartment_atmosphere.hpp>

#include <eve/core/filesystem/filesystem.hpp>
#include <eve/core/logging/logger.hpp>

namespace eve::polish {

bool ApartmentAtmosphere::load(std::string_view data_root) {
    data_root_ = std::string(data_root);
    return true;
}

bool ApartmentAtmosphere::apply_time_preset(world::AnimeRoomLightingSystem& lighting,
                                            TimeOfDayPreset preset) {
    state_.time_preset = preset;
    switch (preset) {
    case TimeOfDayPreset::Morning:
        state_.active_lighting_id = "bedroom_morning";
        state_.lamp_intensity = 0.5f;
        state_.tv_glow = 0.0f;
        break;
    case TimeOfDayPreset::Afternoon:
        state_.active_lighting_id = "living_room_cozy";
        state_.lamp_intensity = 0.4f;
        break;
    case TimeOfDayPreset::Sunset:
        state_.active_lighting_id = "living_room_sunset";
        state_.lamp_intensity = 0.65f;
        break;
    case TimeOfDayPreset::RainyEvening:
        state_.active_lighting_id = "dining_rainy";
        state_.lamp_intensity = 0.75f;
        state_.tv_glow = 0.3f;
        break;
    case TimeOfDayPreset::Night:
        state_.active_lighting_id = "bedroom_night";
        state_.lamp_intensity = 0.35f;
        state_.tv_glow = 0.15f;
        break;
    }
    const std::string path = data_root_ + "/anime/room_lighting.json";
    lighting.load_presets(path);
    return lighting.apply_preset(state_.active_lighting_id);
}

bool ApartmentAtmosphere::apply_room(std::string_view room_id,
                                   world::AnimeRoomLightingSystem& lighting) {
    std::string preset = "wardrobe_cozy_lamp";
    if (room_id == "living_room") {
        preset = state_.time_preset == TimeOfDayPreset::RainyEvening ? "dining_rainy"
                                                                    : "living_room_sunset";
    } else if (room_id == "bedroom") {
        preset = state_.time_preset == TimeOfDayPreset::Night ? "bedroom_night" : "bedroom_morning";
    } else if (room_id == "kitchen" || room_id == "dining_room") {
        preset = "dining_rainy";
    } else if (room_id == "bathroom") {
        preset = "bedroom_night";
    } else if (room_id == "wardrobe") {
        preset = "wardrobe_cozy_lamp";
    }
    state_.active_lighting_id = preset;
    return lighting.apply_preset(preset);
}

void ApartmentAtmosphere::set_lamp_intensity(f32 intensity) {
    state_.lamp_intensity = intensity;
}

void ApartmentAtmosphere::set_tv_glow(f32 intensity) {
    state_.tv_glow = intensity;
}

void ApartmentAtmosphere::trigger_object_sound(std::string_view sound_id) {
    (void)sound_id;
    EVE_LOG(Info, "Atmosphere", "Object sound: ", sound_id);
}

void ApartmentAtmosphere::set_footstep_surface(std::string_view surface) {
    state_.footstep_surface = std::string(surface);
}

} // namespace eve::polish
