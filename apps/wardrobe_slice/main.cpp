#include <cstdlib>
#include <eve/core/logging/logger.hpp>
#include <eve/wardrobe_slice/wardrobe_vertical_slice.hpp>
#include <iostream>

int main(int argc, char** argv) {
    eve::Logger::instance().set_level(eve::LogLevel::Info);
    EVE_LOG(Info, "WardrobeSlice", "EVE Wardrobe Vertical Slice");

    const char* data_root = std::getenv("EVE_DATA_DIR");
    if (data_root == nullptr) {
        data_root = "data";
    }
    if (argc > 1) {
        data_root = argv[1];
    }

    eve::wardrobe_slice::WardrobeVerticalSlice slice;
    if (!slice.initialize(data_root)) {
        std::cerr << "Failed to initialize wardrobe slice\n";
        return 1;
    }

    slice.orbit_camera(0.15f, 0.05f, 0.0f);
    slice.pan_camera(0.0f, 0.02f);
    slice.select_body_region({0.0f, 1.6f, 2.0f}, {0.0f, 0.0f, -1.0f});
    slice.change_outfit("evening_dress");
    slice.load_outfit_preset("casual_day");
    slice.preview_animation("walk");

    for (int frame = 0; frame < 90; ++frame) {
        slice.update(1.0f / 60.0f);
        slice.orbit_camera(0.01f, 0.0f, 0.0f);
    }

    slice.mirror_inspection();
    slice.trigger_romance_reaction("compliment_outfit");
    slice.save_outfit_preset("player_custom");
    slice.capture_photo("wardrobe_screenshot.hdr");

    slice.shutdown();
    EVE_LOG(Info, "WardrobeSlice", "Vertical slice demo complete");
    return 0;
}
