#pragma once

#include <eve/animation/blend_tree.hpp>
#include <eve/animation/facial_animation.hpp>
#include <eve/animation/ik_solver.hpp>
#include <eve/animation/state_machine.hpp>
#include <eve/human/character_interaction_system.hpp>
#include <eve/human/character_profile.hpp>
#include <eve/render/hair_renderer.hpp>
#include <eve/render/human_shader_pipeline.hpp>
#include <eve/render/inspection_camera.hpp>
#include <eve/render/photo_mode.hpp>

namespace eve::human {

class CharacterFramework {
public:
    CharacterFramework();

    bool initialize();
    void shutdown();
    void update(f32 delta_seconds);

    CharacterCreator& creator() { return creator_; }
    CharacterInteractionSystem& interactions() { return interactions_; }
    animation::AnimationStateMachine& animation() { return animation_; }
    animation::BlendTree& blend_tree() { return blend_tree_; }
    animation::IKSolver& ik() { return ik_; }
    animation::FacialAnimationController& facial() { return facial_; }
    render::InspectionCamera& camera() { return camera_; }
    render::HumanShaderPipeline& shader_pipeline() { return shader_pipeline_; }
    render::HairRenderer& hair_renderer() { return hair_renderer_; }
    render::PhotoMode& photo_mode() { return photo_mode_; }

    void setup_default_presets();

private:
    CharacterCreator creator_;
    CharacterInteractionSystem interactions_;
    animation::AnimationStateMachine animation_;
    animation::BlendTree blend_tree_;
    animation::IKSolver ik_;
    animation::FacialAnimationController facial_;
    animation::ProceduralSecondaryMotion secondary_;
    render::InspectionCamera camera_;
    render::HumanShaderPipeline shader_pipeline_;
    render::HairRenderer hair_renderer_;
    render::PhotoMode photo_mode_;
    f32 elapsed_ = 0.0f;
};

} // namespace eve::human
