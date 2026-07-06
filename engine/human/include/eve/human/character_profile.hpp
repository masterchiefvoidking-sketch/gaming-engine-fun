#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::human {

enum class BodyTypePreset : u8 {
    Slim = 0,
    Athletic,
    Curvy,
    Petite,
    Tall,
    Custom,
};

enum class SkinDetailLevel : u8 {
    Low = 0,
    Medium,
    High,
    Ultra,
};

struct BodyProportions {
    f32 height = 1.68f;
    f32 weight = 0.5f;
    f32 shoulder_width = 0.5f;
    f32 bust = 0.5f;
    f32 waist = 0.5f;
    f32 hip = 0.5f;
    f32 leg_length = 0.5f;
    f32 arm_length = 0.5f;
    f32 muscle_definition = 0.3f;
    f32 age_appearance = 0.25f;
    BodyTypePreset body_preset = BodyTypePreset::Custom;
};

struct SkinAppearance {
    math::Vec3 skin_tone{0.92f, 0.78f, 0.68f};
    SkinDetailLevel detail_level = SkinDetailLevel::High;
    f32 freckles = 0.0f;
    f32 beauty_marks = 0.0f;
    f32 subsurface_strength = 0.85f;
    std::vector<std::string> tattoo_ids;
    std::vector<std::string> scar_ids;
};

struct FaceMorphs {
    f32 face_shape = 0.5f;
    f32 jaw = 0.5f;
    f32 chin = 0.5f;
    f32 cheeks = 0.5f;
    f32 lips = 0.5f;
    f32 nose = 0.5f;
    f32 eye_size = 0.5f;
    f32 eye_spacing = 0.5f;
    f32 eyebrow_height = 0.5f;
    f32 eyelash_length = 0.6f;
    f32 makeup_intensity = 0.4f;
};

struct HairProfile {
    std::string style_id = "long_straight";
    f32 length = 0.8f;
    f32 curl = 0.2f;
    math::Vec3 root_color{0.15f, 0.08f, 0.04f};
    math::Vec3 highlight_color{0.35f, 0.22f, 0.12f};
    f32 highlight_strength = 0.3f;
    bool physics_enabled = true;
    bool wind_enabled = true;
    std::vector<std::string> accessory_ids;
};

struct HandCustomization {
    f32 finger_length = 0.5f;
    f32 nail_length = 0.4f;
    math::Vec3 nail_polish_color{0.9f, 0.2f, 0.35f};
    f32 nail_polish_intensity = 0.0f;
    std::vector<std::string> ring_ids;
    std::vector<std::string> bracelet_ids;
};

struct LegFootCustomization {
    f32 leg_proportion = 0.5f;
    f32 foot_size = 0.5f;
    std::string shoe_id = "barefoot";
    std::string sock_id;
    f32 heel_height = 0.0f;
    bool procedural_foot_placement = true;
};

struct CharacterProfile {
    std::string id = "character";
    std::string display_name = "Character";
    BodyProportions body;
    SkinAppearance skin;
    FaceMorphs face;
    HairProfile hair;
    HandCustomization hands;
    LegFootCustomization legs_feet;
    std::unordered_map<std::string, f32> expression_presets;
};

class CharacterCreator {
public:
    void set_profile(CharacterProfile profile);
    [[nodiscard]] const CharacterProfile& profile() const { return profile_; }
    [[nodiscard]] CharacterProfile& profile_mutable() { return profile_; }

    void apply_body_preset(BodyTypePreset preset);
    void set_morph(std::string_view key, f32 value);
    [[nodiscard]] f32 morph(std::string_view key) const;
    [[nodiscard]] math::Mat4 compute_body_transform() const;
    bool save_to_file(std::string_view path) const;
    bool load_from_file(std::string_view path);

private:
    CharacterProfile profile_;
};

} // namespace eve::human
