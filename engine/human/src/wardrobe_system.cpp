#include <eve/core/filesystem/filesystem.hpp>
#include <eve/human/wardrobe_system.hpp>

#include <nlohmann/json.hpp>
#include <random>
#include <algorithm>

namespace eve::human {

const char* to_string(ClothingLayer layer) {
    switch (layer) {
    case ClothingLayer::BaseBody:
        return "base_body";
    case ClothingLayer::SkinDetails:
        return "skin_details";
    case ClothingLayer::Undergarment:
        return "undergarment";
    case ClothingLayer::MainClothing:
        return "main_clothing";
    case ClothingLayer::Outerwear:
        return "outerwear";
    case ClothingLayer::Accessory:
        return "accessory";
    case ClothingLayer::Jewelry:
        return "jewelry";
    default:
        return "unknown";
    }
}

const char* to_string(WardrobeCategory category) {
    switch (category) {
    case WardrobeCategory::Tops:
        return "tops";
    case WardrobeCategory::Bottoms:
        return "bottoms";
    case WardrobeCategory::Dresses:
        return "dresses";
    case WardrobeCategory::Outerwear:
        return "outerwear";
    case WardrobeCategory::Sleepwear:
        return "sleepwear";
    case WardrobeCategory::Formalwear:
        return "formalwear";
    case WardrobeCategory::Casualwear:
        return "casualwear";
    case WardrobeCategory::Athleticwear:
        return "athleticwear";
    case WardrobeCategory::Shoes:
        return "shoes";
    case WardrobeCategory::Socks:
        return "socks";
    case WardrobeCategory::Jewelry:
        return "jewelry";
    case WardrobeCategory::Accessories:
        return "accessories";
    case WardrobeCategory::Makeup:
        return "makeup";
    case WardrobeCategory::Hairstyles:
        return "hairstyles";
    default:
        return "unknown";
    }
}

WardrobeCategory wardrobe_category_from_string(std::string_view value) {
    if (value == "tops") return WardrobeCategory::Tops;
    if (value == "bottoms") return WardrobeCategory::Bottoms;
    if (value == "dresses") return WardrobeCategory::Dresses;
    if (value == "outerwear") return WardrobeCategory::Outerwear;
    if (value == "sleepwear") return WardrobeCategory::Sleepwear;
    if (value == "formalwear") return WardrobeCategory::Formalwear;
    if (value == "casualwear") return WardrobeCategory::Casualwear;
    if (value == "athleticwear") return WardrobeCategory::Athleticwear;
    if (value == "shoes") return WardrobeCategory::Shoes;
    if (value == "socks") return WardrobeCategory::Socks;
    if (value == "jewelry") return WardrobeCategory::Jewelry;
    if (value == "accessories") return WardrobeCategory::Accessories;
    if (value == "makeup") return WardrobeCategory::Makeup;
    if (value == "hairstyles") return WardrobeCategory::Hairstyles;
    return WardrobeCategory::Casualwear;
}

ClothingLayer clothing_layer_from_string(std::string_view value) {
    if (value == "base_body") return ClothingLayer::BaseBody;
    if (value == "skin_details") return ClothingLayer::SkinDetails;
    if (value == "undergarment") return ClothingLayer::Undergarment;
    if (value == "main_clothing") return ClothingLayer::MainClothing;
    if (value == "outerwear") return ClothingLayer::Outerwear;
    if (value == "accessory") return ClothingLayer::Accessory;
    if (value == "jewelry") return ClothingLayer::Jewelry;
    return ClothingLayer::MainClothing;
}

namespace {

ClothingItemDefinition parse_item(const nlohmann::json& json) {
    ClothingItemDefinition item;
    item.id = json.value("id", "");
    item.display_name = json.value("display_name", item.id);
    item.category = wardrobe_category_from_string(json.value("category", "casualwear"));
    item.layer = clothing_layer_from_string(json.value("layer", "main_clothing"));
    item.mesh = json.value("mesh", "");
    item.material = json.value("material", "");
    item.shrinkwrap_offset = json.value("shrinkwrap_offset", 0.002f);
    if (json.contains("coverage_tags")) {
        for (const auto& tag : json["coverage_tags"]) {
            item.coverage_tags.push_back(tag.get<std::string>());
        }
    }
    if (json.contains("compatibility_tags")) {
        for (const auto& tag : json["compatibility_tags"]) {
            item.compatibility_tags.push_back(tag.get<std::string>());
        }
    }
    if (json.contains("incompatible_items")) {
        for (const auto& other : json["incompatible_items"]) {
            item.incompatible_items.push_back(other.get<std::string>());
        }
    }
    if (json.contains("color_variants")) {
        for (const auto& color : json["color_variants"]) {
            item.color_variants.push_back(color.get<std::string>());
        }
    }
    return item;
}

} // namespace

bool WardrobeSystem::load_items(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            register_item(parse_item(entry));
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool WardrobeSystem::load_outfits(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            OutfitPreset preset;
            preset.id = entry.value("id", "");
            preset.name = entry.value("name", preset.id);
            preset.favorite = entry.value("favorite", false);
            if (entry.contains("item_ids")) {
                for (const auto& id : entry["item_ids"]) {
                    preset.item_ids.push_back(id.get<std::string>());
                }
            }
            saved_outfits_.push_back(std::move(preset));
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void WardrobeSystem::register_item(ClothingItemDefinition item) {
    items_.push_back(std::move(item));
}

const ClothingItemDefinition* WardrobeSystem::find_item(std::string_view id) const {
    for (const ClothingItemDefinition& item : items_) {
        if (item.id == id) {
            return &item;
        }
    }
    return nullptr;
}

bool WardrobeSystem::can_equip(std::string_view item_id, const LayeredOutfit& current) const {
    const ClothingItemDefinition* item = find_item(item_id);
    if (item == nullptr) {
        return false;
    }
    for (const std::string& equipped_id : current.equipped_item_ids) {
        if (equipped_id == item_id) {
            return true;
        }
        const ClothingItemDefinition* equipped = find_item(equipped_id);
        if (equipped != nullptr && items_conflict(*item, *equipped)) {
            return false;
        }
    }
    return true;
}

LayeredOutfit WardrobeSystem::equip(std::string_view item_id, LayeredOutfit current) const {
    if (!can_equip(item_id, current)) {
        current.clipping_warnings.push_back("Cannot equip: " + std::string(item_id));
        return current;
    }
    const ClothingItemDefinition* item = find_item(item_id);
    if (item == nullptr) {
        return current;
    }

    std::erase_if(current.equipped_item_ids, [this, item](const std::string& equipped_id) {
        const ClothingItemDefinition* equipped = find_item(equipped_id);
        if (equipped == nullptr) {
            return false;
        }
        return equipped->category == item->category || items_conflict(*item, *equipped);
    });

    current.equipped_item_ids.push_back(std::string(item_id));
    return current;
}

OutfitPreset WardrobeSystem::build_outfit_preset(std::string_view id, const LayeredOutfit& outfit) const {
    OutfitPreset preset;
    preset.id = std::string(id);
    preset.name = std::string(id);
    preset.item_ids = outfit.equipped_item_ids;
    return preset;
}

bool WardrobeSystem::save_outfit(const OutfitPreset& preset, std::string_view path) const {
    nlohmann::json json;
    json["id"] = preset.id;
    json["name"] = preset.name;
    json["favorite"] = preset.favorite;
    json["item_ids"] = preset.item_ids;
    return FileSystem::write_text_file(path, json.dump(2));
}

bool WardrobeSystem::load_outfit(std::string_view path, OutfitPreset& out_preset) const {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        out_preset.id = json.value("id", "");
        out_preset.name = json.value("name", out_preset.id);
        out_preset.favorite = json.value("favorite", false);
        out_preset.item_ids.clear();
        if (json.contains("item_ids")) {
            for (const auto& id : json["item_ids"]) {
                out_preset.item_ids.push_back(id.get<std::string>());
            }
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

OutfitPreset WardrobeSystem::randomize_outfit() const {
    OutfitPreset preset;
    preset.id = "random";
    preset.name = "Random Outfit";
    static std::mt19937 rng{std::random_device{}()};
    std::vector<WardrobeCategory> core = {WardrobeCategory::Tops, WardrobeCategory::Bottoms, WardrobeCategory::Shoes};
    for (WardrobeCategory category : core) {
        std::vector<const ClothingItemDefinition*> candidates;
        for (const ClothingItemDefinition& item : items_) {
            if (item.category == category) {
                candidates.push_back(&item);
            }
        }
        if (!candidates.empty()) {
            std::uniform_int_distribution<std::size_t> dist(0, candidates.size() - 1);
            preset.item_ids.push_back(candidates[dist(rng)]->id);
        }
    }
    return preset;
}

bool WardrobeSystem::coverage_overlaps(const ClothingItemDefinition& a,
                                        const ClothingItemDefinition& b) const {
    for (const std::string& tag : a.coverage_tags) {
        for (const std::string& other : b.coverage_tags) {
            if (tag == other || tag == "full_body" || other == "full_body") {
                return true;
            }
        }
    }
    return false;
}

bool WardrobeSystem::items_conflict(const ClothingItemDefinition& a,
                                    const ClothingItemDefinition& b) const {
    if (a.id == b.id) {
        return false;
    }
    for (const std::string& blocked : a.incompatible_items) {
        if (blocked == b.id) {
            return true;
        }
    }
    for (const std::string& blocked : b.incompatible_items) {
        if (blocked == a.id) {
            return true;
        }
    }
    if (a.category == b.category) {
        return true;
    }
    if (!coverage_overlaps(a, b)) {
        return false;
    }
    const bool a_inner = static_cast<u8>(a.layer) <= static_cast<u8>(ClothingLayer::MainClothing);
    const bool b_inner = static_cast<u8>(b.layer) <= static_cast<u8>(ClothingLayer::MainClothing);
    if (a_inner && b_inner) {
        return true;
    }
    return false;
}

} // namespace eve::human
