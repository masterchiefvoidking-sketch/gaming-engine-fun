#pragma once

#include <eve/content/content_types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::content {

enum class ClothingCategory : u8 {
    Casual = 0,
    Formal,
    Sleepwear,
    Outerwear,
    Shoes,
    Accessories,
    Jewelry,
    Undergarments,
};

struct ClothingItemDefinition {
    ContentMetadata metadata;
    ClothingCategory category = ClothingCategory::Casual;
    std::string mesh_path;
    std::string material_path;
    std::string physics_profile;
    std::string lod_path;
    std::vector<std::string> color_variants;
    std::vector<std::string> compatibility_tags;
    std::string unlock_condition;
    std::string designer_notes;
};

class WardrobeDatabase {
public:
    bool load_item(std::string_view path);
    bool save_item(std::string_view path, const ClothingItemDefinition& item) const;
    void register_item(ClothingItemDefinition item);
    [[nodiscard]] ClothingItemDefinition* find(std::string_view id);
    [[nodiscard]] std::vector<ClothingItemDefinition*> items_in_category(ClothingCategory category);
    [[nodiscard]] std::size_t item_count() const { return items_.size(); }

private:
    std::unordered_map<std::string, ClothingItemDefinition> items_;
};

} // namespace eve::content
