#include <cstdlib>
#include <eve/anime_demo/cozy_wardrobe_demo.hpp>
#include <eve/core/logging/logger.hpp>
#include <iostream>

int main(int argc, char** argv) {
    eve::Logger::instance().set_level(eve::LogLevel::Info);
    EVE_LOG(Info, "AnimeDemo", "Cozy Apartment Wardrobe Preview");

    const char* data_root = std::getenv("EVE_DATA_DIR");
    if (data_root == nullptr) {
        data_root = "data";
    }
    if (argc > 1) {
        data_root = argv[1];
    }

    eve::anime_demo::CozyWardrobeDemo demo;
    if (!demo.initialize(data_root)) {
        std::cerr << "Failed to initialize anime wardrobe demo\n";
        return 1;
    }

    demo.orbit(0.2f, 0.05f, 0.0f);
    demo.apply_full_body();

    for (int frame = 0; frame < 120; ++frame) {
        demo.update(1.0f / 60.0f);
        demo.orbit(0.008f, 0.0f, 0.0f);
    }

    demo.switch_outfit("date_dress");
    demo.switch_hair("wavy");
    demo.switch_expression("blushing");
    demo.apply_face_closeup();

    for (int frame = 0; frame < 60; ++frame) {
        demo.update(1.0f / 60.0f);
    }

    demo.toggle_blush();
    demo.switch_expression("romantic_gaze");
    demo.take_screenshot("anime_wardrobe_screenshot.hdr");

    demo.shutdown();
    EVE_LOG(Info, "AnimeDemo", "Demo complete");
    return 0;
}
