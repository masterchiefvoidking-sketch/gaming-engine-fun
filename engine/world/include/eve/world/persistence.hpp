#pragma once

#include <eve/world/apartment.hpp>
#include <eve/world/interaction.hpp>
#include <eve/world/simulation.hpp>
#include <string>

namespace eve::world {

struct ApartmentSaveData {
    std::string apartment_id;
    ApartmentSimulation simulation;
    std::vector<InteractableComponent> interactables;
    std::vector<RoomComponent> rooms;
};

class ApartmentPersistence {
public:
    bool save(const ApartmentSaveData& data, std::string_view path) const;
    bool load(std::string_view path, ApartmentSaveData& out_data) const;
    bool apply_to_world(const ApartmentSaveData& data, ecs::World& world) const;
    ApartmentSaveData capture_from_world(std::string_view apartment_id, ecs::World& world,
                                         const ApartmentSimulation& simulation) const;
};

} // namespace eve::world
