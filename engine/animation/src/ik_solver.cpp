#include <cmath>
#include <eve/animation/ik_solver.hpp>

namespace eve::animation {

IKSolveResult IKSolver::solve_two_bone(const math::Vec3& root, const math::Vec3& mid,
                                       const math::Vec3& target, f32 upper_length, f32 lower_length,
                                       u32 max_iterations) const {
    (void)mid;
    IKSolveResult result;
    math::Vec3 current = root + math::Vec3{0.0f, upper_length, 0.0f};
    for (u32 i = 0; i < max_iterations; ++i) {
        const math::Vec3 delta = target - current;
        current = current + delta * 0.5f;
        result.iterations = i + 1;
        if (delta.length() < 0.001f) {
            result.converged = true;
            break;
        }
    }
    result.joint_position = current;
    const f32 total = upper_length + lower_length;
    if ((target - root).length() > total) {
        result.converged = false;
    }
    return result;
}

math::Vec3 IKSolver::solve_finger_tip(const math::Vec3& base, const math::Vec3& target,
                                      f32 bone_length) const {
    const math::Vec3 direction = (target - base).normalized();
    return base + direction * bone_length;
}

math::Vec3 IKSolver::solve_look_at(const math::Vec3& head, const math::Vec3& target,
                                   f32 eye_offset_y) const {
    const math::Vec3 eye = head + math::Vec3{0.0f, eye_offset_y, 0.0f};
    return (target - eye).normalized();
}

} // namespace eve::animation
