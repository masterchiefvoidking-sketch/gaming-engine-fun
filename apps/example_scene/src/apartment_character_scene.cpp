#include <eve/core/logging/logger.hpp>
#include <eve/ecs/world.hpp>
#include <eve/example/apartment_character_scene.hpp>
#include <eve/human/character_interaction.hpp>
#include <eve/human/render_sync.hpp>
#include <eve/world/interaction.hpp>

namespace eve::example {

bool ApartmentCharacterScene::initialize(std::string_view data_root) {
    if (!character_.initialize()) {
        EVE_LOG(Error, "Example", "Failed to initialize character framework");
        return false;
    }
    if (!apartment_.load_default_apartment(data_root)) {
        EVE_LOG(Error, "Example", "Failed to load apartment");
        return false;
    }
    if (!apartment_.build_scene()) {
        return false;
    }

    auto& world = apartment_.world();
    world.add_component<human::CharacterInteractionComponent>(player_character_);
    sync_character_to_pipeline(character_.creator().profile(), character_.shader_pipeline());
    character_.camera().apply_preset("full_body");
    EVE_LOG(Info, "Example", "Apartment character scene initialized in room: ", current_room_);
    return true;
}

void ApartmentCharacterScene::update(f32 delta_seconds) {
    apartment_.simulation().update(delta_seconds);
    character_.update(delta_seconds);
}

void ApartmentCharacterScene::shutdown() {
    character_.shutdown();
}

bool ApartmentCharacterScene::navigate_to_room(std::string_view room_id) {
    const world::ApartmentRoom* room = apartment_.layout().find_room(room_id);
    if (room == nullptr) {
        return false;
    }
    current_room_ = std::string(room_id);
    character_.camera().set_focus({0.0f, 1.5f, 0.0f});
    return true;
}

bool ApartmentCharacterScene::interact_with(std::string_view interactable_definition,
                                            human::CharacterInteractionType type) {
    auto& world = apartment_.world();
    EntityId target = kInvalidEntity;
    world.each([&](EntityId entity, ecs::EntityRecord&) {
        if (target != kInvalidEntity) {
            return;
        }
        const auto* item = world.get_component<world::InteractableComponent>(entity);
        if (item != nullptr && item->definition_id == interactable_definition) {
            target = entity;
        }
    });
    if (target == kInvalidEntity) {
        return false;
    }

    const auto result = character_.interactions().resolve(world, player_character_, target, type,
                                                          interactable_definition);
    if (!result.success) {
        return false;
    }

    std::string action_id = "relax";
    if (type == human::CharacterInteractionType::Sit) {
        action_id = "relax";
    } else if (type == human::CharacterInteractionType::Look) {
        action_id = "watch_tv";
    } else if (type == human::CharacterInteractionType::Open) {
        action_id = "lights_on";
    }
    interaction_system_.execute_action(world, target, player_character_, action_id);
    return true;
}

} // namespace eve::example
