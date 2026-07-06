#include <eve/ai/wardrobe.hpp>
#include <random>

namespace eve::ai {

const char* to_string(ClothingCategory category) {
    switch (category) {
    case ClothingCategory::Casual:
        return "casual";
    case ClothingCategory::Business:
        return "business";
    case ClothingCategory::Formal:
        return "formal";
    case ClothingCategory::Athletic:
        return "athletic";
    case ClothingCategory::Sleepwear:
        return "sleepwear";
    case ClothingCategory::Outerwear:
        return "outerwear";
    case ClothingCategory::Shoes:
        return "shoes";
    case ClothingCategory::Jewelry:
        return "jewelry";
    case ClothingCategory::Accessories:
        return "accessories";
    case ClothingCategory::Hairstyles:
        return "hairstyles";
    case ClothingCategory::Cosmetics:
        return "cosmetics";
    default:
        return "unknown";
    }
}

const char* to_string(Mood mood) {
    switch (mood) {
    case Mood::Neutral:
        return "neutral";
    case Mood::Happy:
        return "happy";
    case Mood::Sad:
        return "sad";
    case Mood::Anxious:
        return "anxious";
    case Mood::Romantic:
        return "romantic";
    case Mood::Playful:
        return "playful";
    case Mood::Tired:
        return "tired";
    case Mood::Stressed:
        return "stressed";
    default:
        return "unknown";
    }
}

const char* to_string(SocialIntent intent) {
    switch (intent) {
    case SocialIntent::None:
        return "none";
    case SocialIntent::StartConversation:
        return "start_conversation";
    case SocialIntent::InviteActivity:
        return "invite_activity";
    case SocialIntent::ShareMeal:
        return "share_meal";
    case SocialIntent::WatchMedia:
        return "watch_media";
    case SocialIntent::Comfort:
        return "comfort";
    case SocialIntent::Celebrate:
        return "celebrate";
    case SocialIntent::Exercise:
        return "exercise";
    case SocialIntent::Decorate:
        return "decorate";
    case SocialIntent::ReadTogether:
        return "read_together";
    default:
        return "unknown";
    }
}

std::vector<ClothingItem> WardrobeProfile::items_in_category(ClothingCategory category) const {
    std::vector<ClothingItem> result;
    for (const ClothingItem& item : items) {
        if (item.category == category) {
            result.push_back(item);
        }
    }
    return result;
}

Outfit WardrobeProfile::generate_random_outfit() const {
    Outfit outfit;
    outfit.id = "random_outfit";
    outfit.name = "Random Outfit";

    static std::mt19937 rng{std::random_device{}()};
    const std::vector<ClothingCategory> core_categories = {
        ClothingCategory::Casual,
        ClothingCategory::Shoes,
        ClothingCategory::Accessories,
    };
    for (ClothingCategory category : core_categories) {
        const auto category_items = items_in_category(category);
        if (!category_items.empty()) {
            std::uniform_int_distribution<std::size_t> dist(0, category_items.size() - 1);
            outfit.item_ids.push_back(category_items[dist(rng)].id);
        }
    }
    return outfit;
}

} // namespace eve::ai
