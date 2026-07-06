#include <eve/world/anime_room_lighting.hpp>
#include <gtest/gtest.h>

TEST(AnimeRoomLightingTest, LoadsPresets) {
    eve::world::AnimeRoomLightingSystem lighting;
    ASSERT_TRUE(lighting.load_presets(EVE_DATA_DIR "/anime/room_lighting.json"));
    EXPECT_TRUE(lighting.apply_preset("cozy_lamp"));
}

TEST(AnimeRoomLightingTest, AppliesRoomMood) {
    eve::world::AnimeRoomLightingSystem lighting;
    lighting.load_presets(EVE_DATA_DIR "/anime/room_lighting.json");
    EXPECT_TRUE(lighting.apply_room_mood("living_room", eve::world::AnimeLightingMood::WarmSunset));
}

TEST(AnimeRoomLightingTest, ListsPresetsForRoom) {
    eve::world::AnimeRoomLightingSystem lighting;
    lighting.load_presets(EVE_DATA_DIR "/anime/room_lighting.json");
    const auto ids = lighting.presets_for_room("living_room");
    EXPECT_GE(ids.size(), 1u);
}
