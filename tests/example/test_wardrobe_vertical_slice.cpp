#include <eve/wardrobe_slice/wardrobe_vertical_slice.hpp>
#include <gtest/gtest.h>

TEST(WardrobeVerticalSliceTest, InitializesAndRunsSlice) {
    eve::wardrobe_slice::WardrobeVerticalSlice slice;
    ASSERT_TRUE(slice.initialize(EVE_DATA_DIR));

    slice.orbit_camera(0.1f, 0.0f, 0.0f);
    slice.select_body_region({0.0f, 1.15f, 2.0f}, {0.0f, 0.0f, -1.0f});
    EXPECT_EQ(slice.selected_region(), eve::human::BodyRegion::Torso);

    EXPECT_TRUE(slice.change_outfit("wool_coat"));
    EXPECT_TRUE(slice.load_outfit_preset("evening_elegance"));
    slice.preview_animation("walk");
    slice.mirror_inspection();
    EXPECT_TRUE(slice.trigger_romance_reaction("compliment_outfit"));

    for (int i = 0; i < 30; ++i) {
        slice.update(1.0f / 60.0f);
    }

    slice.shutdown();
}
