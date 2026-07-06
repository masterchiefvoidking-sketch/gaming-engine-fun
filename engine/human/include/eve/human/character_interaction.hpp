#pragma once

#include <eve/core/types.hpp>
#include <string>
#include <vector>

namespace eve::human {

enum class CharacterInteractionType : u8 {
    Reach = 0,
    Grab,
    Sit,
    Stand,
    Walk,
    Lean,
    Look,
    Carry,
    Place,
    Open,
    Close,
    Idle,
};

enum class CharacterInteractionTarget : u8 {
    Mirror = 0,
    Furniture,
    Chair,
    Sofa,
    Bed,
    Door,
    Cabinet,
    Closet,
    Window,
    Appliance,
    Table,
    Book,
    Electronic,
    Decorative,
};

struct CharacterInteractionRequest {
    CharacterInteractionType type = CharacterInteractionType::Idle;
    CharacterInteractionTarget target = CharacterInteractionTarget::Furniture;
    EntityId interactable = kInvalidEntity;
    EntityId character = kInvalidEntity;
    std::string animation_id;
    std::string attach_point;
    f32 duration = 0.0f;
};

struct CharacterInteractionCapabilities {
    bool can_sit = true;
    bool can_grab = true;
    bool can_carry = true;
    bool can_reach = true;
    f32 reach_distance = 1.2f;
    std::vector<CharacterInteractionTarget> supported_targets;
};

struct CharacterInteractionComponent {
    CharacterInteractionCapabilities capabilities;
    CharacterInteractionRequest active_request;
    CharacterInteractionType idle_behavior = CharacterInteractionType::Idle;
    bool is_interacting = false;
    EntityId focused_target = kInvalidEntity;
};

const char* to_string(CharacterInteractionType type);
const char* to_string(CharacterInteractionTarget target);

CharacterInteractionTarget target_from_interactable_id(std::string_view definition_id);

} // namespace eve::human
