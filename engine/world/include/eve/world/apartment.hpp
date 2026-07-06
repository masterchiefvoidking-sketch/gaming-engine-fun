#pragma once

#include <eve/ecs/world.hpp>
#include <eve/world/interaction.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::world {

class InteractionRegistry;

struct ApartmentRoom {
    std::string id;
    std::string display_name;
    RoomType type = RoomType::Hallway;
    math::Vec3 bounds_min{};
    math::Vec3 bounds_max{};
    std::vector<std::string> connected_rooms;
    std::vector<std::string> default_interactables;
    std::vector<std::string> interaction_tags;
};

struct ApartmentLayout {
    std::string id;
    std::string display_name;
    std::unordered_map<std::string, ApartmentRoom> rooms;
    std::string spawn_room;

    [[nodiscard]] const ApartmentRoom* find_room(std::string_view room_id) const;
    [[nodiscard]] std::vector<std::string> room_ids() const;
    [[nodiscard]] std::vector<ApartmentRoom> rooms_of_type(RoomType type) const;
};

class ApartmentLoader {
public:
    bool load_layout(std::string_view path, ApartmentLayout& out_layout) const;
    bool spawn_interactables(const ApartmentLayout& layout, ecs::World& world,
                             InteractionRegistry& registry) const;
};

} // namespace eve::world
