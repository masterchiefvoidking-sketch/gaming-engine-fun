#pragma once

#include <eve/animation/anime_motion.hpp>
#include <eve/human/anime_expression_system.hpp>
#include <eve/render/anime_eye_system.hpp>

#include <string>

namespace eve::polish {

class AnimationPolish {
public:
    void attach(animation::AnimeMotionController& motion, human::AnimeExpressionSystem& expressions,
                render::AnimeEyeSystem& eyes);

    void update(f32 delta_seconds);
    void play_idle();
    void play_sit_transition();
    void play_stand_transition();
    void play_walk_turn();
    void play_mirror_pose();
    void play_outfit_preview_pose();
    void play_dialogue_reaction(std::string_view mood);
    void trigger_hand_gesture(std::string_view gesture_id);
    void set_eye_contact(bool enabled);

    [[nodiscard]] bool is_transitioning() const { return transitioning_; }

private:
    animation::AnimeMotionController* motion_ = nullptr;
    human::AnimeExpressionSystem* expressions_ = nullptr;
    render::AnimeEyeSystem* eyes_ = nullptr;
    f32 transition_timer_ = 0.0f;
    bool transitioning_ = false;
    bool eye_contact_ = true;
};

} // namespace eve::polish
