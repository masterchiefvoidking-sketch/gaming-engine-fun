#pragma once

#include <eve/editor/apartment_builder.hpp>
#include <eve/human/character_framework.hpp>
#include <eve/world/interaction_system.hpp>

namespace eve::example {

class ApartmentCharacterScene {
public:
    bool initialize(std::string_view data_root);
    void update(f32 delta_seconds);
    void shutdown();

    [[nodiscard]] editor::ApartmentBuilder& apartment() { return apartment_; }
    [[nodiscard]] human::CharacterFramework& character() { return character_; }

    bool navigate_to_room(std::string_view room_id);
    bool interact_with(std::string_view interactable_definition,
                       human::CharacterInteractionType type);

private:
    editor::ApartmentBuilder apartment_;
    human::CharacterFramework character_;
    world::InteractionSystem interaction_system_{apartment_.interactions()};
    std::string current_room_ = "hallway";
    EntityId player_character_ = 2;
};

} // namespace eve::example
