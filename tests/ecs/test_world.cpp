#include <eve/ecs/world.hpp>
#include <gtest/gtest.h>
#include <string>

struct Position {
    float x = 0.0f;
    float y = 0.0f;
};

struct Label {
    std::string text;
};

TEST(EcsTest, CreatesEntitiesAndComponents) {
    eve::ecs::ComponentRegistry registry;
    eve::ecs::World world(registry);

    const eve::EntityId entity = world.create_entity();
    EXPECT_TRUE(world.is_alive(entity));

    world.add_component<Position>(entity).x = 3.0f;
    world.add_component<Label>(entity).text = "Actor";

    EXPECT_TRUE(world.has_component<Position>(entity));
    EXPECT_EQ(world.get_component<Label>(entity)->text, "Actor");

    world.destroy_entity(entity);
    EXPECT_FALSE(world.is_alive(entity));
}
