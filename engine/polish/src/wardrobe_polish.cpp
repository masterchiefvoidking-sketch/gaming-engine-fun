#include <eve/polish/wardrobe_polish.hpp>

#include <algorithm>
#include <cstdio>
#include <random>
#include <unordered_map>

namespace eve::polish {

bool WardrobePolish::initialize(std::string_view game_root) {
    const std::string wardrobe_dir =
        std::string(game_root) + "/Assets/Characters/Wardrobe";
    for (int i = 1; i <= 20; ++i) {
        char id[32];
        std::snprintf(id, sizeof(id), "mira_outfit_%02d", i);
        const std::string path = wardrobe_dir + "/" + id + ".json";
        if (content_wardrobe_.load_item(path)) {
            all_outfit_ids_.push_back(id);
            if (content::ClothingItemDefinition* item = content_wardrobe_.find(id)) {
                outfit_categories_[id] = item->category;
            }
        }
    }
    return !all_outfit_ids_.empty();
}

bool WardrobePolish::switch_outfit(human::WardrobeSystem& wardrobe, human::LayeredOutfit& outfit,
                                   std::string_view outfit_id) {
    if (!wardrobe.can_equip(outfit_id, outfit)) {
        state_.compatibility_warnings.push_back("Cannot equip: " + std::string(outfit_id));
        return false;
    }
    outfit = wardrobe.equip(outfit_id, outfit);
    state_.active_outfit = std::string(outfit_id);
    validate_compatibility(wardrobe, outfit);
    return true;
}

bool WardrobePolish::randomize_outfit(human::WardrobeSystem& wardrobe, human::LayeredOutfit& outfit) {
    const auto ids = filtered_outfit_ids();
    if (ids.empty()) {
        return false;
    }
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<std::size_t> dist(0, ids.size() - 1);
    return switch_outfit(wardrobe, outfit, ids[dist(rng)]);
}

bool WardrobePolish::toggle_favorite(std::string_view outfit_id) {
    const std::string id(outfit_id);
    const auto it = std::find(state_.favorites.begin(), state_.favorites.end(), id);
    if (it != state_.favorites.end()) {
        state_.favorites.erase(it);
        return true;
    }
    state_.favorites.push_back(id);
    return true;
}

bool WardrobePolish::save_preset(std::string_view preset_id, const human::LayeredOutfit& outfit,
                                 human::WardrobeSystem& wardrobe) {
    const human::OutfitPreset preset = wardrobe.build_outfit_preset(preset_id, outfit);
    if (!wardrobe.save_outfit(preset, "presets/" + std::string(preset_id) + ".json")) {
        return false;
    }
    state_.saved_presets.push_back(std::string(preset_id));
    return true;
}

bool WardrobePolish::load_preset(std::string_view preset_id, human::WardrobeSystem& wardrobe,
                                 human::LayeredOutfit& outfit) {
    human::OutfitPreset preset;
    if (!wardrobe.load_outfit("presets/" + std::string(preset_id) + ".json", preset)) {
        return false;
    }
    outfit.equipped_item_ids = preset.item_ids;
    state_.active_outfit = preset.id;
    return true;
}

void WardrobePolish::set_filter(WardrobeCategoryFilter filter) {
    state_.filter = filter;
}

bool WardrobePolish::set_color_variant(std::string_view variant) {
    state_.selected_color_variant = std::string(variant);
    return true;
}

void WardrobePolish::validate_compatibility(human::WardrobeSystem& wardrobe,
                                            const human::LayeredOutfit& outfit) {
    state_.compatibility_warnings.clear();
    state_.clipping_warnings = outfit.clipping_warnings;
    for (const std::string& id : outfit.equipped_item_ids) {
        if (wardrobe.find_item(id) == nullptr) {
            state_.compatibility_warnings.push_back("Missing item: " + id);
        }
    }
}

std::vector<std::string> WardrobePolish::filtered_outfit_ids() const {
    if (state_.filter == WardrobeCategoryFilter::All) {
        return all_outfit_ids_;
    }
    std::vector<std::string> filtered;
    for (const std::string& id : all_outfit_ids_) {
        const auto cat_it = outfit_categories_.find(id);
        if (cat_it == outfit_categories_.end()) {
            continue;
        }
        const content::ClothingCategory cat = cat_it->second;
        const bool match =
            (state_.filter == WardrobeCategoryFilter::Casual &&
             cat == content::ClothingCategory::Casual) ||
            (state_.filter == WardrobeCategoryFilter::Formal &&
             cat == content::ClothingCategory::Formal) ||
            (state_.filter == WardrobeCategoryFilter::Sleepwear &&
             cat == content::ClothingCategory::Sleepwear);
        if (match) {
            filtered.push_back(id);
        }
    }
    return filtered.empty() ? all_outfit_ids_ : filtered;
}

} // namespace eve::polish
