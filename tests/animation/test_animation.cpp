#include <eve/animation/blend_tree.hpp>
#include <eve/animation/facial_animation.hpp>
#include <eve/animation/ik_solver.hpp>
#include <eve/animation/state_machine.hpp>
#include <gtest/gtest.h>

TEST(AnimationStateMachineTest, TransitionsOnCondition) {
    eve::animation::AnimationStateMachine machine;
    machine.add_state({eve::animation::AnimationStateId::Idle, "anim_idle"});
    machine.add_state({eve::animation::AnimationStateId::Walk, "anim_walk"});
    machine.add_transition({eve::animation::AnimationStateId::Idle,
                            eve::animation::AnimationStateId::Walk, "walk", 0.2f});
    machine.set_initial(eve::animation::AnimationStateId::Idle);
    machine.update(0.1f, {{"walk", true}});
    EXPECT_EQ(machine.current_state(), eve::animation::AnimationStateId::Idle);
    machine.update(0.2f, {{"walk", true}});
    EXPECT_EQ(machine.current_state(), eve::animation::AnimationStateId::Walk);
}

TEST(BlendTreeTest, ResolvesClip) {
    eve::animation::BlendTree tree;
    tree.add_node({"root", eve::animation::BlendNodeType::Clip, "anim_idle"});
    EXPECT_EQ(tree.resolve_clip("root"), "anim_idle");
}

TEST(IKSolverTest, SolvesFingerTip) {
    eve::animation::IKSolver solver;
    const eve::math::Vec3 tip =
        solver.solve_finger_tip({0.0f, 0.0f, 0.0f}, {0.0f, 0.1f, 0.0f}, 0.08f);
    EXPECT_NEAR(tip.y, 0.08f, 0.01f);
}

TEST(FacialAnimationTest, BlendsMicroExpressions) {
    eve::animation::FacialAnimationController facial;
    facial.add_expression({"smile", 0.0f, 0.0f, 0.9f, 0.0f, 0.5f, 0.0f});
    facial.set_target("smile", 1.0f);
    facial.update(0.016f);
    EXPECT_NEAR(facial.blend_shapes().at("mouth_smile"), 0.9f, 0.01f);
}
