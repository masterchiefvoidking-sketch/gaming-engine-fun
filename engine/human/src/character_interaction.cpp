#include <eve/human/character_interaction.hpp>

namespace eve::human {

const char* to_string(CharacterInteractionType type) {
    switch (type) {
    case CharacterInteractionType::Reach:
        return "reach";
    case CharacterInteractionType::Grab:
        return "grab";
    case CharacterInteractionType::Sit:
        return "sit";
    case CharacterInteractionType::Stand:
        return "stand";
    case CharacterInteractionType::Walk:
        return "walk";
    case CharacterInteractionType::Lean:
        return "lean";
    case CharacterInteractionType::Look:
        return "look";
    case CharacterInteractionType::Carry:
        return "carry";
    case CharacterInteractionType::Place:
        return "place";
    case CharacterInteractionType::Open:
        return "open";
    case CharacterInteractionType::Close:
        return "close";
    default:
        return "idle";
    }
}

const char* to_string(CharacterInteractionTarget target) {
    switch (target) {
    case CharacterInteractionTarget::Mirror:
        return "mirror";
    case CharacterInteractionTarget::Chair:
        return "chair";
    case CharacterInteractionTarget::Sofa:
        return "sofa";
    case CharacterInteractionTarget::Bed:
        return "bed";
    case CharacterInteractionTarget::Door:
        return "door";
    case CharacterInteractionTarget::Appliance:
        return "appliance";
    case CharacterInteractionTarget::Table:
        return "table";
    default:
        return "furniture";
    }
}

CharacterInteractionTarget target_from_interactable_id(std::string_view definition_id) {
    if (definition_id.find("mirror") != std::string::npos) {
        return CharacterInteractionTarget::Mirror;
    }
    if (definition_id.find("bed") != std::string::npos) {
        return CharacterInteractionTarget::Bed;
    }
    if (definition_id.find("sofa") != std::string::npos) {
        return CharacterInteractionTarget::Sofa;
    }
    if (definition_id.find("chair") != std::string::npos) {
        return CharacterInteractionTarget::Chair;
    }
    if (definition_id.find("door") != std::string::npos) {
        return CharacterInteractionTarget::Door;
    }
    if (definition_id.find("stove") != std::string::npos ||
        definition_id.find("fridge") != std::string::npos) {
        return CharacterInteractionTarget::Appliance;
    }
    if (definition_id.find("tv") != std::string::npos) {
        return CharacterInteractionTarget::Electronic;
    }
    if (definition_id.find("table") != std::string::npos) {
        return CharacterInteractionTarget::Table;
    }
    return CharacterInteractionTarget::Furniture;
}

} // namespace eve::human
