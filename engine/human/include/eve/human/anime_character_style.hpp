#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>

#include <string>
#include <unordered_map>

namespace eve::human {

enum class AnimeSilhouettePreset : u8 {
    Petite = 0,
    Balanced,
    Curvy,
    TallElegant,
    Custom,
};

struct AnimeBodyProportions {
    f32 height = 1.58f;
    f32 head_to_body_ratio = 0.22f;
    f32 shoulder_width = 0.38f;
    f32 waist = 0.34f;
    f32 hip = 0.42f;
    f32 bust = 0.40f;
    f32 leg_length = 0.52f;
    f32 arm_length = 0.46f;
    f32 hand_size = 0.42f;
    f32 foot_size = 0.40f;
    f32 posture_tilt = 0.0f;
    AnimeSilhouettePreset silhouette = AnimeSilhouettePreset::Balanced;
};

struct AnimeCharacterStyle {
    std::string id = "default_anime";
    std::string display_name = "Original Anime Heroine";
    AnimeBodyProportions body;
    math::Vec3 hair_color{0.22f, 0.14f, 0.28f};
    math::Vec3 eye_color{0.38f, 0.58f, 0.78f};
    std::string default_hair_style = "long_straight";
    std::string default_material = "warm_romance";
};

class AnimeCharacterStyleSystem {
public:
    bool load_from_file(std::string_view path);
    void register_style(AnimeCharacterStyle style);
    bool apply_style(std::string_view style_id);
    void apply_silhouette_preset(AnimeSilhouettePreset preset);

    [[nodiscard]] const AnimeCharacterStyle& active() const { return active_; }
    [[nodiscard]] math::Mat4 compute_transform() const;

private:
    AnimeCharacterStyle active_{};
    std::unordered_map<std::string, AnimeCharacterStyle> styles_;
};

} // namespace eve::human
