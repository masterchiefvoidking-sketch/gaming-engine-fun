#include <eve/core/filesystem/filesystem.hpp>
#include <eve/world/apartment.hpp>
#include <eve/world/interaction_system.hpp>
#include <nlohmann/json.hpp>

namespace eve::world {

const ApartmentRoom* ApartmentLayout::find_room(std::string_view room_id) const {
    const auto it = rooms.find(std::string(room_id));
    if (it == rooms.end()) {
        return nullptr;
    }
    return &it->second;
}

std::vector<std::string> ApartmentLayout::room_ids() const {
    std::vector<std::string> ids;
    ids.reserve(rooms.size());
    for (const auto& [id, _] : rooms) {
        ids.push_back(id);
    }
    return ids;
}

std::vector<ApartmentRoom> ApartmentLayout::rooms_of_type(RoomType type) const {
    std::vector<ApartmentRoom> result;
    for (const auto& [_, room] : rooms) {
        if (room.type == type) {
            result.push_back(room);
        }
    }
    return result;
}

bool ApartmentLoader::load_layout(std::string_view path, ApartmentLayout& out_layout) const {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        out_layout.id = json.value("id", "apartment");
        out_layout.display_name = json.value("display_name", out_layout.id);
        out_layout.spawn_room = json.value("spawn_room", "hallway");
        out_layout.rooms.clear();

        if (!json.contains("rooms")) {
            return false;
        }

        for (const auto& room_json : json["rooms"]) {
            ApartmentRoom room;
            room.id = room_json.value("id", "");
            room.display_name = room_json.value("display_name", room.id);
            room.type = room_type_from_string(room_json.value("type", "hallway"));

            if (room_json.contains("bounds")) {
                const auto& bounds = room_json["bounds"];
                room.bounds_min = {bounds.value("min_x", 0.0f), bounds.value("min_y", 0.0f),
                                   bounds.value("min_z", 0.0f)};
                room.bounds_max = {bounds.value("max_x", 0.0f), bounds.value("max_y", 0.0f),
                                   bounds.value("max_z", 0.0f)};
            }
            if (room_json.contains("connected_rooms")) {
                for (const auto& connected : room_json["connected_rooms"]) {
                    room.connected_rooms.push_back(connected.get<std::string>());
                }
            }
            if (room_json.contains("default_interactables")) {
                for (const auto& item : room_json["default_interactables"]) {
                    room.default_interactables.push_back(item.get<std::string>());
                }
            }
            if (room_json.contains("interaction_tags")) {
                for (const auto& tag : room_json["interaction_tags"]) {
                    room.interaction_tags.push_back(tag.get<std::string>());
                }
            }
            out_layout.rooms.emplace(room.id, std::move(room));
        }
        return !out_layout.rooms.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool ApartmentLoader::spawn_interactables(const ApartmentLayout& layout, ecs::World& world,
                                          InteractionRegistry& registry) const {
    (void)registry;
    for (const auto& [room_id, room] : layout.rooms) {
        const EntityId room_entity = world.create_entity();
        auto& room_component = world.add_component<RoomComponent>(room_entity);
        room_component.room_id = room_id;
        room_component.display_name = room.display_name;
        room_component.type = room.type;
        room_component.bounds_min = room.bounds_min;
        room_component.bounds_max = room.bounds_max;
        room_component.connected_rooms = room.connected_rooms;
        room_component.interaction_tags = room.interaction_tags;

        for (const std::string& interactable_id : room.default_interactables) {
            const EntityId object_entity = world.create_entity();
            auto& interactable = world.add_component<InteractableComponent>(object_entity);
            interactable.definition_id = interactable_id;
            interactable.display_name = interactable_id;
            interactable.room = room.type;
        }
    }
    return true;
}

} // namespace eve::world
