#include <eve/content/wardrobe_database.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::content {

bool WardrobeDatabase::load_item(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        ClothingItemDefinition item;
        item.metadata.id = json.value("id", "");
        item.metadata.display_name = json.value("name", "");
        item.metadata.thumbnail_path = json.value("thumbnail", "");
        item.metadata.collection = json.value("collection", "");
        item.mesh_path = json.value("mesh", "");
        item.material_path = json.value("material", "");
        item.physics_profile = json.value("physics", "default_cloth");
        item.lod_path = json.value("lod", "");
        item.unlock_condition = json.value("unlock", "");
        item.designer_notes = json.value("designer_notes", "");
        if (json.contains("color_variants")) {
            for (const auto& c : json["color_variants"]) {
                item.color_variants.push_back(c.get<std::string>());
            }
        }
        if (json.contains("tags")) {
            for (const auto& t : json["tags"]) {
                item.metadata.tags.push_back(t.get<std::string>());
                item.compatibility_tags.push_back(t.get<std::string>());
            }
        }
        const std::string cat = json.value("category", "casual");
        if (cat == "formal") item.category = ClothingCategory::Formal;
        else if (cat == "sleepwear") item.category = ClothingCategory::Sleepwear;
        else if (cat == "outerwear") item.category = ClothingCategory::Outerwear;
        register_item(std::move(item));
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool WardrobeDatabase::save_item(std::string_view path,
                                 const ClothingItemDefinition& item) const {
    nlohmann::json json;
    json["id"] = item.metadata.id;
    json["name"] = item.metadata.display_name;
    json["thumbnail"] = item.metadata.thumbnail_path;
    json["mesh"] = item.mesh_path;
    json["material"] = item.material_path;
    json["physics"] = item.physics_profile;
    json["tags"] = item.metadata.tags;
    json["color_variants"] = item.color_variants;
    return FileSystem::write_text_file(path, json.dump(2));
}

void WardrobeDatabase::register_item(ClothingItemDefinition item) {
    items_[item.metadata.id] = std::move(item);
}

ClothingItemDefinition* WardrobeDatabase::find(std::string_view id) {
    const auto it = items_.find(std::string(id));
    if (it == items_.end()) {
        return nullptr;
    }
    return &it->second;
}

std::vector<ClothingItemDefinition*> WardrobeDatabase::items_in_category(
    ClothingCategory category) {
    std::vector<ClothingItemDefinition*> result;
    for (auto& [_, item] : items_) {
        if (item.category == category) {
            result.push_back(&item);
        }
    }
    return result;
}

} // namespace eve::content
