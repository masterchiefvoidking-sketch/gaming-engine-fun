#include <eve/world/apartment.hpp>
#include <eve/world/interaction.hpp>
#include <eve/world/interaction_system.hpp>
#include <gtest/gtest.h>

TEST(InteractionTest, LoadsDefinitionsAndExecutesAction) {
    eve::ecs::ComponentRegistry registry;
    eve::ecs::World world(registry);
    registry.register_component<eve::world::InteractableComponent>();

    eve::world::InteractionRegistry interaction_registry;
    ASSERT_TRUE(
        interaction_registry.load_from_file(EVE_DATA_DIR "/interactions/room_interactions.json"));
    EXPECT_GE(interaction_registry.definition_ids().size(), 5u);

    eve::world::InteractionSystem system(interaction_registry);
    const eve::EntityId object = world.create_entity();
    auto& bed = world.add_component<eve::world::InteractableComponent>(object);
    bed.definition_id = "bed";
    bed.display_name = "Bed";
    bed.room = eve::world::RoomType::Bedroom;
    bed.position = {0.0f, 0.0f, 0.0f};

    const eve::EntityId actor = world.create_entity();
    const auto result = system.execute_action(world, object, actor, "sleep");
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.animation_id, "anim_sleep");
    EXPECT_EQ(bed.state, eve::world::ObjectState::InUse);
}

TEST(InteractionTest, HoverQueryFindsNearestObject) {
    eve::ecs::ComponentRegistry registry;
    eve::ecs::World world(registry);
    registry.register_component<eve::world::InteractableComponent>();

    eve::world::InteractionRegistry interaction_registry;
    interaction_registry.load_from_file(EVE_DATA_DIR "/interactions/room_interactions.json");
    eve::world::InteractionSystem system(interaction_registry);

    const eve::EntityId near = world.create_entity();
    world.add_component<eve::world::InteractableComponent>(near).position = {0.1f, 0.0f, 0.0f};

    const eve::EntityId far = world.create_entity();
    world.add_component<eve::world::InteractableComponent>(far).position = {5.0f, 0.0f, 0.0f};

    const auto hover = system.query_hover(world, {0.0f, 0.0f, 0.0f}, 1.0f);
    ASSERT_TRUE(hover.valid);
    EXPECT_EQ(hover.entity, near);
}
