#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::human {

enum class ClothingLayer : u8 {
    BaseBody = 0,
    SkinDetails,
    Undergarment,
    MainClothing,
    Outerwear,
    Accessory,
    Jewelry,
    Count
};

enum class WardrobeCategory : u8 {
    Tops = 0,
    Bottoms,
    Dresses,
    Outerwear,
    Sleepwear,
    Formalwear,
    Casualwear,
    Athleticwear,
    Shoes,
    Socks,
    Jewelry,
    Accessories,
    Makeup,
    Hairstyles,
    Count
};

enum class ClothingState : u8 {
    Clean = 0,
    Dirty,
    Wet,
    Damaged,
};

struct ClothingPhysicsProfile {
    f32 stiffness = 0.5f;
    f32 damping = 0.3f;
    f32 collision_radius = 0.02f;
    bool simulate = true;
};

struct ClothingItemDefinition {
    std::string id;
    std::string display_name;
    WardrobeCategory category = WardrobeCategory::Casualwear;
    ClothingLayer layer = ClothingLayer::MainClothing;
    std::string mesh;
    std::string material;
    ClothingPhysicsProfile physics;
    std::vector<std::string> coverage_tags;
    std::vector<std::string> compatibility_tags;
    std::vector<std::string> incompatible_items;
    std::vector<std::string> color_variants;
    f32 shrinkwrap_offset = 0.002f;
    ClothingState state = ClothingState::Clean;
};

struct OutfitPreset {
    std::string id;
    std::string name;
    std::vector<std::string> item_ids;
    bool favorite = false;
    std::unordered_map<std::string, f32> morph_adjustments;
};

struct LayeredOutfit {
    std::vector<std::string> equipped_item_ids;
    std::vector<std::string> clipping_warnings;
};

class WardrobeSystem {
public:
    bool load_items(std::string_view path);
    bool load_outfits(std::string_view path);
    void register_item(ClothingItemDefinition item);

    [[nodiscard]] const ClothingItemDefinition* find_item(std::string_view id) const;
    [[nodiscard]] bool can_equip(std::string_view item_id, const LayeredOutfit& current) const;
    [[nodiscard]] LayeredOutfit equip(std::string_view item_id, LayeredOutfit current) const;
    [[nodiscard]] OutfitPreset build_outfit_preset(std::string_view id, const LayeredOutfit& outfit) const;

    bool save_outfit(const OutfitPreset& preset, std::string_view path) const;
    bool load_outfit(std::string_view path, OutfitPreset& out_preset) const;
    [[nodiscard]] OutfitPreset randomize_outfit() const;

    [[nodiscard]] const std::vector<ClothingItemDefinition>& items() const { return items_; }
    [[nodiscard]] const std::vector<OutfitPreset>& saved_outfits() const { return saved_outfits_; }

private:
    [[nodiscard]] bool items_conflict(const ClothingItemDefinition& a, const ClothingItemDefinition& b) const;
    [[nodiscard]] bool coverage_overlaps(const ClothingItemDefinition& a,
                                         const ClothingItemDefinition& b) const;

    std::vector<ClothingItemDefinition> items_;
    std::vector<OutfitPreset> saved_outfits_;
};

const char* to_string(ClothingLayer layer);
const char* to_string(WardrobeCategory category);
WardrobeCategory wardrobe_category_from_string(std::string_view value);
ClothingLayer clothing_layer_from_string(std::string_view value);

} // namespace eve::human
