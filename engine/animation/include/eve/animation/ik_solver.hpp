#pragma once

#include <eve/core/math/math.hpp>

namespace eve::animation {

struct IKSolveResult {
    math::Vec3 joint_position;
    math::Quat joint_rotation = math::Quat::identity();
    bool converged = false;
    u32 iterations = 0;
};

class IKSolver {
public:
    IKSolveResult solve_two_bone(const math::Vec3& root, const math::Vec3& mid,
                                 const math::Vec3& target, f32 upper_length, f32 lower_length,
                                 u32 max_iterations = 8) const;

    math::Vec3 solve_finger_tip(const math::Vec3& base, const math::Vec3& target,
                                f32 bone_length) const;
    math::Vec3 solve_look_at(const math::Vec3& head, const math::Vec3& target,
                             f32 eye_offset_y = 0.05f) const;
};

} // namespace eve::animation
