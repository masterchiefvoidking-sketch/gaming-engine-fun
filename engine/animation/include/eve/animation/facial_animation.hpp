#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>
#include <string>
#include <unordered_map>

namespace eve::animation {

struct FacialMicroExpression {
    std::string id;
    f32 brow_raise = 0.0f;
    f32 brow_lower = 0.0f;
    f32 mouth_smile = 0.0f;
    f32 mouth_press = 0.0f;
    f32 cheek_raise = 0.0f;
    f32 blink = 0.0f;
};

class FacialAnimationController {
public:
    void add_expression(FacialMicroExpression expression);
    void set_target(std::string_view expression_id, f32 weight);
    void update(f32 delta_seconds);

    [[nodiscard]] const std::unordered_map<std::string, f32>& blend_shapes() const {
        return blend_shapes_;
    }

private:
    std::unordered_map<std::string, FacialMicroExpression> expressions_;
    std::unordered_map<std::string, f32> targets_;
    std::unordered_map<std::string, f32> blend_shapes_;
};

class ProceduralSecondaryMotion {
public:
    void update_breathing(f32 delta_seconds, f32& chest_offset, f32 intensity = 0.02f);
    void update_idle_sway(f32 time, math::Vec3& offset, f32 intensity = 0.01f);
};

} // namespace eve::animation
