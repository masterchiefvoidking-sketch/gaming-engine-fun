#include <eve/world/apartment.hpp>
#include <eve/world/interaction_system.hpp>
#include <gtest/gtest.h>

TEST(ApartmentTest, LoadsAllRooms) {
    eve::world::ApartmentLoader loader;
    eve::world::ApartmentLayout layout;
    ASSERT_TRUE(loader.load_layout(EVE_DATA_DIR "/apartments/default/layout.json", layout));
    EXPECT_EQ(layout.rooms.size(), 11u);
    EXPECT_NE(layout.find_room("bedroom"), nullptr);
    EXPECT_NE(layout.find_room("kitchen"), nullptr);
    EXPECT_NE(layout.find_room("wardrobe"), nullptr);
}

TEST(ApartmentTest, SpawnsRoomEntities) {
    eve::world::ApartmentLoader loader;
    eve::world::ApartmentLayout layout;
    eve::world::InteractionRegistry registry;
    loader.load_layout(EVE_DATA_DIR "/apartments/default/layout.json", layout);

    eve::ecs::ComponentRegistry component_registry;
    eve::ecs::World world(component_registry);
    component_registry.register_component<eve::world::RoomComponent>();
    component_registry.register_component<eve::world::InteractableComponent>();

    ASSERT_TRUE(loader.spawn_interactables(layout, world, registry));
    EXPECT_GE(world.entity_count(), 11u);
}
