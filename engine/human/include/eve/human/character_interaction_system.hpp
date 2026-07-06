#pragma once

#include <eve/ecs/world.hpp>
#include <eve/human/character_interaction.hpp>

namespace eve::human {

struct InteractionResolveResult {
    bool success = false;
    CharacterInteractionRequest request;
    std::string message;
};

class CharacterInteractionSystem {
public:
    InteractionResolveResult resolve(ecs::World& world, EntityId character, EntityId interactable,
                                     CharacterInteractionType type, std::string_view definition_id);

    void complete(ecs::World& world, EntityId character);
    [[nodiscard]] bool can_perform(const CharacterInteractionComponent& character,
                                   CharacterInteractionType type,
                                   CharacterInteractionTarget target) const;
};

} // namespace eve::human
