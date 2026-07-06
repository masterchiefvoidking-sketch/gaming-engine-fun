#include <eve/core/filesystem/filesystem.hpp>
#include <eve/world/persistence.hpp>
#include <nlohmann/json.hpp>

namespace eve::world {

namespace {

nlohmann::json interactable_to_json(const InteractableComponent& c) {
    return {
        {"definition_id", c.definition_id},
        {"display_name", c.display_name},
        {"room", to_string(c.room)},
        {"state", to_string(c.state)},
        {"position", {{"x", c.position.x}, {"y", c.position.y}, {"z", c.position.z}}},
        {"custom_state", c.custom_state},
    };
}

InteractableComponent interactable_from_json(const nlohmann::json& json) {
    InteractableComponent c;
    c.definition_id = json.value("definition_id", "");
    c.display_name = json.value("display_name", c.definition_id);
    c.room = room_type_from_string(json.value("room", "hallway"));
    c.state = ObjectState::Idle;
    if (json.contains("position")) {
        c.position.x = json["position"].value("x", 0.0f);
        c.position.y = json["position"].value("y", 0.0f);
        c.position.z = json["position"].value("z", 0.0f);
    }
    if (json.contains("custom_state")) {
        for (const auto& [key, value] : json["custom_state"].items()) {
            c.custom_state[key] = value.get<std::string>();
        }
    }
    return c;
}

} // namespace

bool ApartmentPersistence::save(const ApartmentSaveData& data, std::string_view path) const {
    nlohmann::json json;
    json["apartment_id"] = data.apartment_id;
    json["simulation"] = {
        {"hour", data.simulation.clock.hour},
        {"day_index", data.simulation.clock.day_index},
        {"weather", to_string(data.simulation.clock.weather)},
        {"cleanliness", data.simulation.cleanliness},
        {"pantry_freshness", data.simulation.pantry_freshness},
    };

    nlohmann::json interactables = nlohmann::json::array();
    for (const InteractableComponent& item : data.interactables) {
        interactables.push_back(interactable_to_json(item));
    }
    json["interactables"] = interactables;

    return FileSystem::write_text_file(path, json.dump(2));
}

bool ApartmentPersistence::load(std::string_view path, ApartmentSaveData& out_data) const {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        out_data.apartment_id = json.value("apartment_id", "");
        if (json.contains("simulation")) {
            const auto& sim = json["simulation"];
            out_data.simulation.clock.hour = sim.value("hour", 8.0f);
            out_data.simulation.clock.day_index = sim.value("day_index", 0);
            out_data.simulation.cleanliness = sim.value("cleanliness", 1.0f);
            out_data.simulation.pantry_freshness = sim.value("pantry_freshness", 1.0f);
        }
        out_data.interactables.clear();
        if (json.contains("interactables")) {
            for (const auto& item : json["interactables"]) {
                out_data.interactables.push_back(interactable_from_json(item));
            }
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool ApartmentPersistence::apply_to_world(const ApartmentSaveData& data, ecs::World& world) const {
    for (const InteractableComponent& saved : data.interactables) {
        const EntityId entity = world.create_entity();
        world.add_component<InteractableComponent>(entity) = saved;
    }
    return true;
}

ApartmentSaveData
ApartmentPersistence::capture_from_world(std::string_view apartment_id, ecs::World& world,
                                         const ApartmentSimulation& simulation) const {
    ApartmentSaveData data;
    data.apartment_id = std::string(apartment_id);
    data.simulation = simulation;

    world.each([&](EntityId entity, ecs::EntityRecord&) {
        if (const auto* interactable = world.get_component<InteractableComponent>(entity)) {
            data.interactables.push_back(*interactable);
        }
        if (const auto* room = world.get_component<RoomComponent>(entity)) {
            data.rooms.push_back(*room);
        }
    });
    return data;
}

} // namespace eve::world
