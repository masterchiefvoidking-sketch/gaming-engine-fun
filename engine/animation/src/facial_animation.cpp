#include <algorithm>
#include <cmath>
#include <eve/animation/facial_animation.hpp>

namespace eve::animation {

void FacialAnimationController::add_expression(FacialMicroExpression expression) {
    expressions_[expression.id] = std::move(expression);
}

void FacialAnimationController::set_target(std::string_view expression_id, f32 weight) {
    targets_[std::string(expression_id)] = std::clamp(weight, 0.0f, 1.0f);
}

void FacialAnimationController::update(f32 delta_seconds) {
    (void)delta_seconds;
    blend_shapes_.clear();
    for (const auto& [id, weight] : targets_) {
        const auto it = expressions_.find(id);
        if (it == expressions_.end()) {
            continue;
        }
        const FacialMicroExpression& expr = it->second;
        blend_shapes_["brow_raise"] = expr.brow_raise * weight;
        blend_shapes_["brow_lower"] = expr.brow_lower * weight;
        blend_shapes_["mouth_smile"] = expr.mouth_smile * weight;
        blend_shapes_["mouth_press"] = expr.mouth_press * weight;
        blend_shapes_["cheek_raise"] = expr.cheek_raise * weight;
        blend_shapes_["blink"] = expr.blink * weight;
    }
}

void ProceduralSecondaryMotion::update_breathing(f32 delta_seconds, f32& chest_offset,
                                                 f32 intensity) {
    static f32 time = 0.0f;
    time += delta_seconds;
    chest_offset = std::sin(time * 2.0f) * intensity;
}

void ProceduralSecondaryMotion::update_idle_sway(f32 time, math::Vec3& offset, f32 intensity) {
    offset.x = std::sin(time * 0.7f) * intensity;
    offset.z = std::cos(time * 0.5f) * intensity * 0.5f;
}

} // namespace eve::animation
