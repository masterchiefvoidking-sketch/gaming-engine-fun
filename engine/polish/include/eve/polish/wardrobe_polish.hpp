#pragma once

#include <eve/content/wardrobe_database.hpp>
#include <eve/human/wardrobe_system.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::polish {

enum class WardrobeCategoryFilter : u8 {
    All = 0,
    Casual,
    Formal,
    Sleepwear,
    Athletic,
    Dresses,
    Shoes,
    Accessories,
};

struct WardrobePolishState {
    std::string active_outfit;
    std::string comparison_outfit;
    std::vector<std::string> favorites;
    std::vector<std::string> saved_presets;
    WardrobeCategoryFilter filter = WardrobeCategoryFilter::All;
    std::string selected_color_variant = "default";
    std::vector<std::string> compatibility_warnings;
    std::vector<std::string> clipping_warnings;
};

class WardrobePolish {
public:
    bool initialize(std::string_view game_root);
    bool switch_outfit(human::WardrobeSystem& wardrobe, human::LayeredOutfit& outfit,
                       std::string_view outfit_id);
    bool randomize_outfit(human::WardrobeSystem& wardrobe, human::LayeredOutfit& outfit);
    bool toggle_favorite(std::string_view outfit_id);
    bool save_preset(std::string_view preset_id, const human::LayeredOutfit& outfit,
                     human::WardrobeSystem& wardrobe);
    bool load_preset(std::string_view preset_id, human::WardrobeSystem& wardrobe,
                     human::LayeredOutfit& outfit);
    void set_filter(WardrobeCategoryFilter filter);
    bool set_color_variant(std::string_view variant);
    void validate_compatibility(human::WardrobeSystem& wardrobe, const human::LayeredOutfit& outfit);

    [[nodiscard]] const WardrobePolishState& state() const { return state_; }
    [[nodiscard]] std::vector<std::string> filtered_outfit_ids() const;

private:
    WardrobePolishState state_;
    content::WardrobeDatabase content_wardrobe_;
    std::vector<std::string> all_outfit_ids_;
    std::unordered_map<std::string, content::ClothingCategory> outfit_categories_;
};

} // namespace eve::polish
