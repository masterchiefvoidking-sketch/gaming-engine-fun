#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::render {

struct ToonRampStop {
    f32 threshold = 0.0f;
    math::Vec3 color{1.0f, 1.0f, 1.0f};
};

struct AnimeSkinParams {
    math::Vec3 base_tone{0.98f, 0.88f, 0.82f};
    math::Vec3 shadow_tone{0.85f, 0.72f, 0.68f};
    f32 blush_intensity = 0.0f;
    math::Vec3 blush_color{0.95f, 0.55f, 0.58f};
    f32 soft_shadow_band = 0.35f;
    f32 rim_light_intensity = 0.45f;
    math::Vec3 rim_light_color{1.0f, 0.92f, 0.85f};
    f32 wetness_shine = 0.0f;
    math::Vec3 mood_tint{1.0f, 1.0f, 1.0f};
};

struct AnimeMakeupParams {
    f32 lip_tint_intensity = 0.35f;
    math::Vec3 lip_color{0.85f, 0.35f, 0.42f};
    f32 eyeshadow_intensity = 0.2f;
    math::Vec3 eyeshadow_color{0.55f, 0.35f, 0.45f};
    f32 eyelash_darkness = 0.7f;
};

struct AnimeOutlineParams {
    f32 thickness = 1.2f;
    math::Vec3 color{0.12f, 0.08f, 0.14f};
    f32 depth_bias = 0.002f;
};

struct AnimeMaterialPreset {
    std::string id;
    std::string label;
    AnimeSkinParams skin;
    AnimeMakeupParams makeup;
    AnimeOutlineParams outline;
    std::vector<ToonRampStop> toon_ramp;
};

class AnimeHumanMaterial {
public:
    bool initialize();
    void shutdown();

    void load_presets(std::string_view path);
    void register_preset(AnimeMaterialPreset preset);
    bool apply_preset(std::string_view preset_id);

    void set_skin_params(const AnimeSkinParams& skin);
    void set_makeup_params(const AnimeMakeupParams& makeup);
    void set_outline_params(const AnimeOutlineParams& outline);
    void set_blush(f32 intensity);
    void set_mood_tint(const math::Vec3& tint);
    void set_wetness(f32 shine);

    [[nodiscard]] bool is_ready() const { return ready_; }
    [[nodiscard]] const AnimeSkinParams& skin() const { return skin_; }
    [[nodiscard]] const AnimeMakeupParams& makeup() const { return makeup_; }
    [[nodiscard]] const AnimeOutlineParams& outline() const { return outline_; }
    [[nodiscard]] const std::vector<ToonRampStop>& toon_ramp() const { return toon_ramp_; }
    [[nodiscard]] const AnimeMaterialPreset* find_preset(std::string_view id) const;

private:
    bool ready_ = false;
    AnimeSkinParams skin_{};
    AnimeMakeupParams makeup_{};
    AnimeOutlineParams outline_{};
    std::vector<ToonRampStop> toon_ramp_;
    std::unordered_map<std::string, AnimeMaterialPreset> presets_;
};

[[nodiscard]] f32 evaluate_toon_ramp(f32 ndotl, const std::vector<ToonRampStop>& ramp);

} // namespace eve::render
