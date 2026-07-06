#include <eve/world/persistence.hpp>
#include <filesystem>
#include <gtest/gtest.h>

TEST(PersistenceTest, SavesAndLoadsApartmentState) {
    eve::ecs::ComponentRegistry registry;
    eve::ecs::World world(registry);
    registry.register_component<eve::world::InteractableComponent>();

    const eve::EntityId entity = world.create_entity();
    auto& bed = world.add_component<eve::world::InteractableComponent>(entity);
    bed.definition_id = "bed";
    bed.display_name = "Bed";

    eve::world::ApartmentSimulation simulation;
    simulation.clock.hour = 22.5f;
    simulation.cleanliness = 0.8f;

    eve::world::ApartmentPersistence persistence;
    const auto data = persistence.capture_from_world("default_apartment", world, simulation);

    const std::string path = "eve_save_test.json";
    ASSERT_TRUE(persistence.save(data, path));

    eve::world::ApartmentSaveData loaded;
    ASSERT_TRUE(persistence.load(path, loaded));
    EXPECT_EQ(loaded.apartment_id, "default_apartment");
    EXPECT_NEAR(loaded.simulation.clock.hour, 22.5f, 0.01f);
    EXPECT_EQ(loaded.interactables.size(), 1u);

    std::filesystem::remove(path);
}

TEST(PersistenceTest, CapturesWorldState) {
    eve::ecs::ComponentRegistry registry;
    eve::ecs::World world(registry);
    registry.register_component<eve::world::InteractableComponent>();
    registry.register_component<eve::world::RoomComponent>();

    const eve::EntityId entity = world.create_entity();
    world.add_component<eve::world::InteractableComponent>(entity).definition_id = "bed";

    eve::world::ApartmentSimulation simulation;
    eve::world::ApartmentPersistence persistence;
    const auto data = persistence.capture_from_world("default_apartment", world, simulation);
    EXPECT_EQ(data.interactables.size(), 1u);
}
