#include <eve/animation/layer_stack.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::animation {

void AnimationLayerStack::load_definitions(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            AnimationLayerDefinition def;
            def.name = entry.value("name", "");
            def.priority = entry.value("priority", 0);
            def.default_weight = entry.value("default_weight", 1.0f);
            def.additive = entry.value("additive", false);
            def.default_clip = entry.value("default_clip", "anim_idle");
            const std::string id_str = entry.value("id", "base_locomotion");
            if (id_str == "upper_body_gestures") {
                def.id = AnimationLayerId::UpperBodyGestures;
            } else if (id_str == "facial_expressions") {
                def.id = AnimationLayerId::FacialExpressions;
            } else if (id_str == "eye_movement") {
                def.id = AnimationLayerId::EyeMovement;
            } else if (id_str == "hair_physics") {
                def.id = AnimationLayerId::HairPhysics;
            } else if (id_str == "cloth_physics") {
                def.id = AnimationLayerId::ClothPhysics;
            } else if (id_str == "hand_finger") {
                def.id = AnimationLayerId::HandFinger;
            } else if (id_str == "breathing") {
                def.id = AnimationLayerId::Breathing;
            } else if (id_str == "idle_movement") {
                def.id = AnimationLayerId::IdleMovement;
            } else if (id_str == "prop_interaction") {
                def.id = AnimationLayerId::PropInteraction;
            } else if (id_str == "camera_facing_pose") {
                def.id = AnimationLayerId::CameraFacingPose;
            } else {
                def.id = AnimationLayerId::BaseLocomotion;
            }
            add_definition(def);
        }
    } catch (const nlohmann::json::exception&) {
    }
}

void AnimationLayerStack::add_definition(AnimationLayerDefinition definition) {
    const AnimationLayerId id = definition.id;
    definitions_[id] = std::move(definition);
    AnimationLayerState state;
    state.id = id;
    state.active_clip = definitions_[id].default_clip;
    state.weight = definitions_[id].default_weight;
    states_[id] = state;
}

void AnimationLayerStack::set_layer_clip(AnimationLayerId id, std::string clip_id, f32 weight) {
    auto it = states_.find(id);
    if (it == states_.end()) {
        return;
    }
    it->second.active_clip = std::move(clip_id);
    it->second.weight = weight;
}

void AnimationLayerStack::update(f32 delta_seconds) {
    (void)delta_seconds;
}

f32 AnimationLayerStack::blended_weight(AnimationLayerId id) const {
    const auto it = states_.find(id);
    if (it == states_.end() || !it->second.enabled) {
        return 0.0f;
    }
    return it->second.weight;
}

std::vector<AnimationLayerState> AnimationLayerStack::active_layers() const {
    std::vector<AnimationLayerState> layers;
    for (const auto& [_, state] : states_) {
        if (state.enabled && state.weight > 0.0f) {
            layers.push_back(state);
        }
    }
    return layers;
}

} // namespace eve::animation
