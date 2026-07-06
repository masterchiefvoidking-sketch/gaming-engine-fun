#pragma once

#include <eve/ecs/world.hpp>
#include <eve/world/interaction.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::world {

class InteractionRegistry {
public:
    void register_definition(InteractionDefinition definition);
    [[nodiscard]] const InteractionDefinition* find(std::string_view id) const;
    [[nodiscard]] std::vector<std::string> definition_ids() const;
    bool load_from_file(std::string_view path);
    bool load_from_directory(std::string_view directory);

private:
    std::unordered_map<std::string, InteractionDefinition> definitions_;
};

struct InteractionResult {
    bool success = false;
    std::string action_id;
    std::string animation_id;
    std::string sound_id;
    ObjectState new_state = ObjectState::Idle;
    f32 duration_seconds = 0.0f;
    std::string message;
};

class InteractionSystem {
public:
    explicit InteractionSystem(InteractionRegistry& registry);

    [[nodiscard]] HoverResult query_hover(ecs::World& world, const math::Vec3& point,
                                          f32 radius = 0.5f) const;

    InteractionResult execute_action(ecs::World& world, EntityId interactable, EntityId actor,
                                     std::string_view action_id);

    [[nodiscard]] std::vector<ContextAction>
    available_actions(ecs::World& world, EntityId interactable, EntityId actor) const;

    void set_hovered(ecs::World& world, EntityId interactable, bool hovered);
    void release_interactable(ecs::World& world, EntityId interactable);

private:
    [[nodiscard]] bool can_execute(const ContextAction& action, const InteractableComponent& target,
                                   EntityId actor) const;

    InteractionRegistry& registry_;
};

} // namespace eve::world
