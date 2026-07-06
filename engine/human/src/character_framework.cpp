#include <eve/human/character_framework.hpp>
#include <eve/human/render_sync.hpp>
#include <unordered_map>

namespace eve::human {

CharacterFramework::CharacterFramework()
    : hair_renderer_(shader_pipeline_)
    , photo_mode_(camera_, shader_pipeline_) {}

bool CharacterFramework::initialize() {
    if (!shader_pipeline_.initialize()) {
        return false;
    }
    hair_renderer_.initialize();
    setup_default_presets();
    creator_.apply_body_preset(BodyTypePreset::Curvy);

    animation_.add_state({animation::AnimationStateId::Idle, "anim_idle", true, 1.0f, 0});
    animation_.add_state({animation::AnimationStateId::Walk, "anim_walk", true, 1.0f, 0});
    animation_.add_state({animation::AnimationStateId::Sit, "anim_sit", false, 1.0f, 0});
    animation_.add_transition({animation::AnimationStateId::Idle, animation::AnimationStateId::Walk,
                               "is_walking", 0.25f});
    animation_.add_transition(
        {animation::AnimationStateId::Walk, animation::AnimationStateId::Idle, "is_idle", 0.25f});
    animation_.set_initial(animation::AnimationStateId::Idle);

    blend_tree_.add_node({"locomotion", animation::BlendNodeType::Blend1D, "anim_idle", 0.0f});
    blend_tree_.add_node({"walk", animation::BlendNodeType::Clip, "anim_walk"});

    facial_.add_expression({"smile", 0.0f, 0.0f, 0.8f, 0.0f, 0.4f, 0.0f});
    facial_.add_expression({"blink", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f});

    return true;
}

void CharacterFramework::shutdown() {
    shader_pipeline_.shutdown();
}

void CharacterFramework::update(f32 delta_seconds) {
    elapsed_ += delta_seconds;
    std::unordered_map<std::string, bool> conditions{{"is_idle", true}};
    animation_.update(delta_seconds, conditions);
    facial_.update(delta_seconds);
    hair_renderer_.update(delta_seconds, {0.0f, 1.65f, 0.0f});
    camera_.update(delta_seconds);
}

void CharacterFramework::setup_default_presets() {
    camera_.add_preset({"portrait", "Portrait", {0.0f, 1.6f, 1.4f}, {0.0f, 1.55f, 0.0f}, 35.0f});
    camera_.add_preset({"full_body", "Full Body", {0.0f, 1.2f, 3.5f}, {0.0f, 1.0f, 0.0f}, 40.0f});
    camera_.add_preset({"fashion", "Fashion", {1.2f, 1.3f, 2.8f}, {0.0f, 1.1f, 0.0f}, 32.0f});

    photo_mode_.add_lighting_preset(
        {"studio_soft", "Studio Soft", {0.3f, -0.8f, 0.4f}, 2.5f, 0.6f, 1.2f});
    photo_mode_.add_lighting_preset(
        {"golden_hour", "Golden Hour", {-0.6f, -0.2f, 0.7f}, 2.0f, 0.4f, 1.5f});
}

} // namespace eve::human
