#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>

#include <string>

namespace eve::animation {

enum class AnimeBodyLanguageId : u8 {
    Neutral = 0,
    Shy,
    Confident,
    Happy,
    Embarrassed,
    IdleBreathing,
    MirrorPose,
    OutfitPreviewWalk,
};

struct AnimeMotionState {
    AnimeBodyLanguageId body_language = AnimeBodyLanguageId::Neutral;
    f32 breathing_offset = 0.0f;
    f32 hair_sway = 0.0f;
    f32 cloth_sway = 0.0f;
    f32 head_tilt = 0.0f;
    f32 shoulder_offset = 0.0f;
    std::string active_clip = "anim_idle";
};

class AnimeMotionController {
public:
    void set_body_language(AnimeBodyLanguageId language);
    void trigger_reaction(std::string_view reaction_id);
    void set_eye_tracking(bool enabled);
    void update(f32 delta_seconds);

    [[nodiscard]] const AnimeMotionState& state() const { return state_; }

private:
    AnimeMotionState state_{};
    f32 elapsed_ = 0.0f;
    bool eye_tracking_ = true;
};

} // namespace eve::animation
