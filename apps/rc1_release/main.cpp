#include <eve/rc1_release/rc1_release_app.hpp>

#include <eve/core/logging/logger.hpp>

#include <cstdlib>

int main() {
    constexpr const char* kDataRoot = EVE_DATA_DIR;
    constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";

    eve::rc1_release::RC1ReleaseApp app;
    if (!app.initialize(kDataRoot, kGameRoot)) {
        EVE_LOG(Error, "RC1Release", "Failed to initialize RC1 pipeline");
        return EXIT_FAILURE;
    }

    const eve::polish::RC1ReleaseReport report = app.run();
    if (!report.success) {
        EVE_LOG(Error, "RC1Release", report.message);
        return EXIT_FAILURE;
    }

    EVE_LOG(Info, "RC1Release", "RC1 build complete: ", report.builds.size(), " profiles exported");
    return EXIT_SUCCESS;
}
