#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <unordered_map>

namespace eve::human {

enum class AnimeExpressionId : u8 {
    Neutral = 0,
    Smile,
    SoftSmile,
    Laugh,
    Embarrassed,
    Blushing,
    Shy,
    Annoyed,
    Sad,
    Surprised,
    Flirty,
    Sleepy,
    Curious,
    Pouting,
    Confident,
    RomanticGaze,
    Count
};

struct AnimeExpressionChannels {
    f32 brow_raise = 0.0f;
    f32 brow_lower = 0.0f;
    f32 eye_open = 1.0f;
    f32 eye_squint = 0.0f;
    f32 pupil_scale = 1.0f;
    f32 mouth_smile = 0.0f;
    f32 mouth_open = 0.0f;
    f32 cheek_raise = 0.0f;
    f32 blush = 0.0f;
    f32 head_tilt = 0.0f;
    f32 shoulder_pose = 0.0f;
    std::string voice_reaction_hook;
};

struct AnimeExpressionPreset {
    std::string id;
    AnimeExpressionId expression = AnimeExpressionId::Neutral;
    AnimeExpressionChannels channels;
};

class AnimeExpressionSystem {
public:
    bool load_presets(std::string_view path);
    void register_preset(AnimeExpressionPreset preset);
    bool set_expression(std::string_view expression_id, f32 blend_weight = 1.0f);
    bool set_expression(AnimeExpressionId expression, f32 blend_weight = 1.0f);
    void update(f32 delta_seconds);

    [[nodiscard]] const AnimeExpressionChannels& active_channels() const { return blended_; }
    [[nodiscard]] bool blush_enabled() const { return blended_.blush > 0.01f; }
    [[nodiscard]] const AnimeExpressionPreset* find(std::string_view id) const;

private:
    AnimeExpressionChannels blend_toward(const AnimeExpressionChannels& target, f32 weight) const;

    std::unordered_map<std::string, AnimeExpressionPreset> presets_;
    AnimeExpressionChannels blended_{};
    AnimeExpressionChannels target_{};
    f32 blend_speed_ = 6.0f;
};

AnimeExpressionId expression_from_string(std::string_view value);
const char* to_string(AnimeExpressionId expression);

} // namespace eve::human
