#include <eve/ecs/world.hpp>
#include <eve/human/character_interaction_system.hpp>
#include <gtest/gtest.h>

TEST(CharacterInteractionTest, ResolvesSitOnSofa) {
    eve::ecs::ComponentRegistry registry;
    eve::ecs::World world(registry);
    registry.register_component<eve::human::CharacterInteractionComponent>();

    const eve::EntityId character = world.create_entity();
    world.add_component<eve::human::CharacterInteractionComponent>(character);

    const eve::EntityId sofa = world.create_entity();
    eve::human::CharacterInteractionSystem system;
    const auto result =
        system.resolve(world, character, sofa, eve::human::CharacterInteractionType::Sit, "sofa");
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.request.animation_id, "anim_sit");
}

TEST(CharacterInteractionTest, MapsInteractableTargets) {
    EXPECT_EQ(eve::human::target_from_interactable_id("mirror"),
              eve::human::CharacterInteractionTarget::Mirror);
    EXPECT_EQ(eve::human::target_from_interactable_id("tv_living"),
              eve::human::CharacterInteractionTarget::Electronic);
}
