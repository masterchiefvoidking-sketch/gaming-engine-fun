#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>
#include <eve/world/apartment_types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::world {

enum class AnimeLightingMood : u8 {
    Morning = 0,
    WarmSunset,
    CozyLamp,
    Night,
    RainyDay,
    NeonCity,
};

struct AnimeRoomLightingPreset {
    std::string id;
    std::string room_id;
    AnimeLightingMood mood = AnimeLightingMood::CozyLamp;
    math::Vec3 key_light_color{1.0f, 0.92f, 0.82f};
    math::Vec3 fill_light_color{0.7f, 0.75f, 0.9f};
    math::Vec3 rim_light_color{1.0f, 0.85f, 0.7f};
    f32 key_intensity = 1.2f;
    f32 fill_intensity = 0.4f;
    f32 rim_intensity = 0.35f;
    f32 window_beam_strength = 0.0f;
    bool mirror_reflections = true;
    bool soft_shadows = true;
};

class AnimeRoomLightingSystem {
public:
    bool load_presets(std::string_view path);
    void register_preset(AnimeRoomLightingPreset preset);
    bool apply_room_mood(std::string_view room_id, AnimeLightingMood mood);
    bool apply_preset(std::string_view preset_id);

    [[nodiscard]] const AnimeRoomLightingPreset& active() const { return active_; }
    [[nodiscard]] std::vector<std::string> presets_for_room(std::string_view room_id) const;

private:
    AnimeRoomLightingPreset active_{};
    std::unordered_map<std::string, AnimeRoomLightingPreset> presets_;
};

} // namespace eve::world
