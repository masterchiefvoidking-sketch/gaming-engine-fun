#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>
#include <eve/world/apartment_types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::world {

struct ContextAction {
    std::string id;
    std::string label;
    InteractionCategory category = InteractionCategory::General;
    std::string animation_id;
    std::string sound_id;
    std::string resulting_state;
    f32 duration_seconds = 0.0f;
    bool ai_usable = true;
    bool requires_empty_hands = false;
    std::vector<std::string> required_items;
    std::vector<std::string> tags;
};

struct InteractionDefinition {
    std::string id;
    std::string display_name;
    RoomType room = RoomType::Hallway;
    std::vector<ContextAction> actions;
    std::vector<std::string> supported_states;
    std::string default_state = "idle";
};

struct InteractableComponent {
    std::string definition_id;
    std::string display_name;
    RoomType room = RoomType::Hallway;
    ObjectState state = ObjectState::Idle;
    math::Vec3 position{};
    bool hoverable = true;
    bool hovered = false;
    EntityId current_user = kInvalidEntity;
    std::unordered_map<std::string, std::string> custom_state;
};

struct RoomComponent {
    std::string room_id;
    std::string display_name;
    RoomType type = RoomType::Hallway;
    math::Vec3 bounds_min{};
    math::Vec3 bounds_max{};
    f32 ambient_light = 1.0f;
    f32 temperature = 22.0f;
    bool lights_on = true;
    std::vector<std::string> connected_rooms;
    std::vector<std::string> interaction_tags;
};

struct HoverResult {
    EntityId entity = kInvalidEntity;
    std::string interactable_name;
    std::vector<ContextAction> available_actions;
    bool valid = false;
};

} // namespace eve::world
