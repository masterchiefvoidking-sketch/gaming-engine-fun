#include <eve/editor/apartment_builder.hpp>
#include <filesystem>
#include <gtest/gtest.h>

TEST(ApartmentBuilderTest, LoadsAndBuildsDefaultApartment) {
    eve::editor::ApartmentBuilder builder;
    ASSERT_TRUE(builder.load_default_apartment(EVE_DATA_DIR));
    ASSERT_TRUE(builder.build_scene());
    EXPECT_EQ(builder.layout().rooms.size(), 11u);
    EXPECT_GE(builder.world().entity_count(), 11u);
}

TEST(ApartmentBuilderTest, SavesAndLoadsState) {
    eve::editor::ApartmentBuilder builder;
    ASSERT_TRUE(builder.load_default_apartment(EVE_DATA_DIR));
    ASSERT_TRUE(builder.build_scene());

    const std::string path = "eve_apartment_save.json";
    ASSERT_TRUE(builder.save_state(path));

    eve::editor::ApartmentBuilder loaded;
    ASSERT_TRUE(loaded.load_default_apartment(EVE_DATA_DIR));
    ASSERT_TRUE(loaded.load_state(path));
    std::filesystem::remove(path);
}
