#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::animation {

enum class AnimationLayerId : u8 {
    BaseLocomotion = 0,
    UpperBodyGestures,
    FacialExpressions,
    EyeMovement,
    HairPhysics,
    ClothPhysics,
    HandFinger,
    Breathing,
    IdleMovement,
    PropInteraction,
    CameraFacingPose,
    Count
};

struct AnimationLayerDefinition {
    AnimationLayerId id = AnimationLayerId::BaseLocomotion;
    std::string name;
    u32 priority = 0;
    f32 default_weight = 1.0f;
    bool additive = false;
    std::string default_clip;
};

struct AnimationLayerState {
    AnimationLayerId id = AnimationLayerId::BaseLocomotion;
    std::string active_clip;
    f32 weight = 1.0f;
    f32 playback_speed = 1.0f;
    bool enabled = true;
};

class AnimationLayerStack {
public:
    void load_definitions(std::string_view path);
    void add_definition(AnimationLayerDefinition definition);
    void set_layer_clip(AnimationLayerId id, std::string clip_id, f32 weight = 1.0f);
    void update(f32 delta_seconds);

    [[nodiscard]] f32 blended_weight(AnimationLayerId id) const;
    [[nodiscard]] std::vector<AnimationLayerState> active_layers() const;

private:
    std::unordered_map<AnimationLayerId, AnimationLayerDefinition> definitions_;
    std::unordered_map<AnimationLayerId, AnimationLayerState> states_;
};

} // namespace eve::animation
