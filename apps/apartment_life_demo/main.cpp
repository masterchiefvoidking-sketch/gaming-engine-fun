#include <eve/apartment_demo/apartment_life_demo_app.hpp>

#include <eve/core/logging/logger.hpp>

#include <cstdlib>

int main() {
    constexpr const char* kDataRoot = EVE_DATA_DIR;
    constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";

    eve::apartment_demo::ApartmentLifeDemoApp app;
    if (!app.initialize(kDataRoot, kGameRoot)) {
        EVE_LOG(Error, "ApartmentLifeDemo", "Failed to initialize demo");
        return EXIT_FAILURE;
    }

    const bool slice_ok = app.run_vertical_slice();
    app.shutdown();

    if (!slice_ok) {
        EVE_LOG(Error, "ApartmentLifeDemo", "Vertical slice flow failed");
        return EXIT_FAILURE;
    }

    EVE_LOG(Info, "ApartmentLifeDemo", "Apartment Life vertical slice completed successfully");
    return EXIT_SUCCESS;
}
