#include <eve/human/dressing_room.hpp>
#include <eve/human/wardrobe_system.hpp>
#include <gtest/gtest.h>

TEST(WardrobeSystemTest, LayeringPreventsDressOverJeans) {
    eve::human::WardrobeSystem wardrobe;
    ASSERT_TRUE(wardrobe.load_items(EVE_DATA_DIR "/wardrobe/items.json"));

    eve::human::LayeredOutfit outfit;
    outfit = wardrobe.equip("denim_jeans", outfit);
    EXPECT_FALSE(wardrobe.can_equip("evening_dress", outfit));

    outfit = {};
    outfit = wardrobe.equip("evening_dress", outfit);
    EXPECT_FALSE(wardrobe.can_equip("silk_blouse", outfit));
    EXPECT_FALSE(wardrobe.can_equip("denim_jeans", outfit));
}

TEST(WardrobeSystemTest, CompatibilityAllowsCasualStack) {
    eve::human::WardrobeSystem wardrobe;
    ASSERT_TRUE(wardrobe.load_items(EVE_DATA_DIR "/wardrobe/items.json"));

    eve::human::LayeredOutfit outfit;
    EXPECT_TRUE(wardrobe.can_equip("silk_blouse", outfit));
    outfit = wardrobe.equip("silk_blouse", outfit);
    EXPECT_TRUE(wardrobe.can_equip("denim_jeans", outfit));
    outfit = wardrobe.equip("denim_jeans", outfit);
    EXPECT_TRUE(wardrobe.can_equip("stiletto_heels", outfit));
}

TEST(WardrobeSystemTest, OutfitSaveAndLoad) {
    eve::human::WardrobeSystem wardrobe;
    ASSERT_TRUE(wardrobe.load_items(EVE_DATA_DIR "/wardrobe/items.json"));

    eve::human::LayeredOutfit outfit;
    outfit = wardrobe.equip("silk_blouse", outfit);
    outfit = wardrobe.equip("denim_jeans", outfit);
    const eve::human::OutfitPreset preset = wardrobe.build_outfit_preset("test_outfit", outfit);
    const std::string path = "test_outfit_save.json";
    ASSERT_TRUE(wardrobe.save_outfit(preset, path));

    eve::human::OutfitPreset loaded;
    ASSERT_TRUE(wardrobe.load_outfit(path, loaded));
    EXPECT_EQ(loaded.id, "test_outfit");
    EXPECT_EQ(loaded.item_ids.size(), 2u);
}

TEST(WardrobeSystemTest, LoadsSavedOutfits) {
    eve::human::WardrobeSystem wardrobe;
    ASSERT_TRUE(wardrobe.load_items(EVE_DATA_DIR "/wardrobe/items.json"));
    ASSERT_TRUE(wardrobe.load_outfits(EVE_DATA_DIR "/wardrobe/outfits.json"));
    EXPECT_EQ(wardrobe.saved_outfits().size(), 3u);
}

TEST(DressingRoomTest, TryOnRespectsCompatibility) {
    eve::human::WardrobeSystem wardrobe;
    wardrobe.load_items(EVE_DATA_DIR "/wardrobe/items.json");
    eve::human::CharacterFramework character;
    character.initialize();
    eve::human::BodyInteractionRegistry registry;
    eve::human::BodyInteractionSystem interactions(registry);
    eve::human::DressingRoomController room(wardrobe, character, interactions);

    EXPECT_TRUE(room.try_on("evening_dress"));
    EXPECT_FALSE(room.try_on("denim_jeans"));
    character.shutdown();
}
