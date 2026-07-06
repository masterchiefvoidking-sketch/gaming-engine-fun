#include <eve/content/apartment_content.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::content {

bool ApartmentContentLibrary::load_apartment(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        ApartmentContentDefinition def;
        def.metadata.id = json.value("id", "");
        def.metadata.display_name = json.value("name", "");
        if (json.contains("rooms")) {
            for (const auto& room : json["rooms"]) {
                def.room_ids.push_back(room.get<std::string>());
            }
        }
        if (json.contains("furniture")) {
            for (const auto& item : json["furniture"]) {
                FurniturePlacement placement;
                placement.furniture_id = item.value("id", "");
                placement.room_id = item.value("room", "");
                placement.x = item.value("x", 0.0f);
                placement.y = item.value("y", 0.0f);
                placement.z = item.value("z", 0.0f);
                def.furniture.push_back(placement);
            }
        }
        register_apartment(std::move(def));
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool ApartmentContentLibrary::load_furniture(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        ObjectContentDefinition def;
        def.metadata.id = json.value("id", "");
        def.metadata.display_name = json.value("name", "");
        def.physics_profile = json.value("physics", "static");
        def.blocks_navigation = json.value("blocks_nav", false);
        def.contributes_lighting = json.value("emits_light", false);
        if (json.contains("interactions")) {
            for (const auto& inter : json["interactions"]) {
                ObjectInteractionDef interaction;
                interaction.id = inter.value("id", "");
                interaction.label = inter.value("label", "");
                interaction.animation_id = inter.value("animation", "");
                interaction.sound_id = inter.value("sound", "");
                def.interactions.push_back(std::move(interaction));
            }
        }
        register_furniture(std::move(def));
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void ApartmentContentLibrary::register_apartment(ApartmentContentDefinition def) {
    apartments_[def.metadata.id] = std::move(def);
}

void ApartmentContentLibrary::register_furniture(ObjectContentDefinition def) {
    furniture_[def.metadata.id] = std::move(def);
}

ApartmentContentDefinition* ApartmentContentLibrary::find_apartment(std::string_view id) {
    const auto it = apartments_.find(std::string(id));
    return it != apartments_.end() ? &it->second : nullptr;
}

ObjectContentDefinition* ApartmentContentLibrary::find_furniture(std::string_view id) {
    const auto it = furniture_.find(std::string(id));
    return it != furniture_.end() ? &it->second : nullptr;
}

} // namespace eve::content
