#pragma once

#include <eve/core/types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::ai {

enum class ClothingCategory : u8 {
    Casual = 0,
    Business,
    Formal,
    Athletic,
    Sleepwear,
    Outerwear,
    Shoes,
    Jewelry,
    Accessories,
    Hairstyles,
    Cosmetics,
    Count
};

enum class Mood : u8 {
    Neutral = 0,
    Happy,
    Sad,
    Anxious,
    Romantic,
    Playful,
    Tired,
    Stressed,
};

enum class SocialIntent : u8 {
    None = 0,
    StartConversation,
    InviteActivity,
    ShareMeal,
    WatchMedia,
    Comfort,
    Celebrate,
    Exercise,
    Decorate,
    ReadTogether,
};

struct ClothingItem {
    std::string id;
    std::string display_name;
    ClothingCategory category = ClothingCategory::Casual;
    std::string color;
    std::vector<std::string> tags;
};

struct Outfit {
    std::string id;
    std::string name;
    std::vector<std::string> item_ids;
    bool favorite = false;
};

struct WardrobeProfile {
    std::vector<ClothingItem> items;
    std::vector<Outfit> saved_outfits;
    std::vector<std::string> favorites;
    std::string active_outfit_id;

    [[nodiscard]] std::vector<ClothingItem> items_in_category(ClothingCategory category) const;
    [[nodiscard]] Outfit generate_random_outfit() const;
};

const char* to_string(ClothingCategory category);
const char* to_string(Mood mood);
const char* to_string(SocialIntent intent);

} // namespace eve::ai
