#include <eve/human/character_interaction_system.hpp>

namespace eve::human {

InteractionResolveResult CharacterInteractionSystem::resolve(ecs::World& world, EntityId character,
                                                             EntityId interactable,
                                                             CharacterInteractionType type,
                                                             std::string_view definition_id) {
    InteractionResolveResult result;
    auto* component = world.get_component<CharacterInteractionComponent>(character);
    if (component == nullptr) {
        result.message = "Character missing interaction component";
        return result;
    }

    const CharacterInteractionTarget target = target_from_interactable_id(definition_id);
    if (!can_perform(*component, type, target)) {
        result.message = "Interaction not supported";
        return result;
    }

    CharacterInteractionRequest request;
    request.type = type;
    request.target = target;
    request.character = character;
    request.interactable = interactable;
    request.duration = 1.0f;

    switch (type) {
    case CharacterInteractionType::Sit:
        request.animation_id = "anim_sit";
        request.attach_point = "sit_point";
        break;
    case CharacterInteractionType::Grab:
        request.animation_id = "anim_grab";
        break;
    case CharacterInteractionType::Look:
        request.animation_id = "anim_look";
        break;
    case CharacterInteractionType::Open:
        request.animation_id = "anim_open";
        break;
    default:
        request.animation_id = "anim_idle";
        break;
    }

    component->active_request = request;
    component->is_interacting = true;
    component->focused_target = interactable;

    result.success = true;
    result.request = request;
    result.message = "Resolved interaction";
    return result;
}

void CharacterInteractionSystem::complete(ecs::World& world, EntityId character) {
    if (auto* component = world.get_component<CharacterInteractionComponent>(character)) {
        component->is_interacting = false;
        component->focused_target = kInvalidEntity;
        component->active_request = {};
        component->active_request.type = component->idle_behavior;
    }
}

bool CharacterInteractionSystem::can_perform(const CharacterInteractionComponent& character,
                                             CharacterInteractionType type,
                                             CharacterInteractionTarget target) const {
    switch (type) {
    case CharacterInteractionType::Sit:
        return character.capabilities.can_sit && (target == CharacterInteractionTarget::Chair ||
                                                  target == CharacterInteractionTarget::Sofa ||
                                                  target == CharacterInteractionTarget::Bed);
    case CharacterInteractionType::Grab:
    case CharacterInteractionType::Carry:
        return character.capabilities.can_grab;
    case CharacterInteractionType::Reach:
    case CharacterInteractionType::Look:
        return character.capabilities.can_reach;
    default:
        return true;
    }
}

} // namespace eve::human
