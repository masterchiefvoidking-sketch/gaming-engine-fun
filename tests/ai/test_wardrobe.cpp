#include <eve/ai/wardrobe.hpp>
#include <gtest/gtest.h>

TEST(WardrobeTest, FiltersCategoriesAndBuildsOutfit) {
    eve::ai::WardrobeProfile wardrobe;
    wardrobe.items = {
        {"shirt_casual", "Casual Shirt", eve::ai::ClothingCategory::Casual, "blue", {}},
        {"shoes_black", "Black Shoes", eve::ai::ClothingCategory::Shoes, "black", {}},
        {"watch", "Watch", eve::ai::ClothingCategory::Accessories, "silver", {}},
    };

    EXPECT_EQ(wardrobe.items_in_category(eve::ai::ClothingCategory::Casual).size(), 1u);
    const eve::ai::Outfit outfit = wardrobe.generate_random_outfit();
    EXPECT_FALSE(outfit.item_ids.empty());
}
