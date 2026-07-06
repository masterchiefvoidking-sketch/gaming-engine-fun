#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::render {

enum class EyeGazeMode : u8 {
    Direct = 0,
    ShyGlance,
    SideGlance,
    LookAway,
    LookAtTarget,
};

enum class EyeEmotionShape : u8 {
    Neutral = 0,
    Happy,
    Shy,
    Surprised,
    Sad,
    Flirty,
    Annoyed,
    Sleepy,
};

struct EyeHighlightLayer {
    math::Vec2 offset{0.2f, 0.25f};
    f32 scale = 0.15f;
    f32 intensity = 0.9f;
    math::Vec3 color{1.0f, 1.0f, 1.0f};
};

struct AnimeEyePreset {
    std::string id;
    std::string label;
    math::Vec3 iris_color{0.35f, 0.55f, 0.75f};
    f32 pupil_scale = 0.55f;
    f32 iris_scale = 1.15f;
    f32 gloss_intensity = 0.85f;
    std::vector<EyeHighlightLayer> highlights;
};

struct AnimeEyeState {
    EyeGazeMode gaze = EyeGazeMode::Direct;
    EyeEmotionShape emotion = EyeEmotionShape::Neutral;
    f32 blink_amount = 0.0f;
    f32 wink_left = 0.0f;
    f32 wink_right = 0.0f;
    f32 tear_gloss = 0.0f;
    math::Vec2 look_target{0.0f, 0.0f};
    f32 eye_contact_timer = 0.0f;
    bool sparkle_enabled = true;
};

class AnimeEyeSystem {
public:
    bool initialize();
    void load_presets(std::string_view path);
    void register_preset(AnimeEyePreset preset);
    bool apply_preset(std::string_view preset_id);

    void set_gaze(EyeGazeMode gaze);
    void set_emotion(EyeEmotionShape emotion);
    void set_look_target(const math::Vec2& target);
    void trigger_blink();
    void trigger_wink(bool left_eye);
    void update(f32 delta_seconds);

    [[nodiscard]] const AnimeEyeState& state() const { return state_; }
    [[nodiscard]] const AnimeEyePreset& active_preset() const { return active_preset_; }
    [[nodiscard]] f32 pupil_scale() const;
    [[nodiscard]] math::Vec2 gaze_offset() const;

private:
    AnimeEyeState state_{};
    AnimeEyePreset active_preset_{};
    std::unordered_map<std::string, AnimeEyePreset> presets_;
    f32 blink_timer_ = 0.0f;
    f32 next_blink_ = 3.0f;
    bool blinking_ = false;
};

} // namespace eve::render
