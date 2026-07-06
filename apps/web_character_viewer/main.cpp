#include <eve/web_viewer/web_character_viewer_app.hpp>

#include <eve/core/logging/logger.hpp>

#include <cstdlib>
#include <string>

int main(int argc, char** argv) {
    std::string data_root = EVE_DATA_DIR;
    std::string game_root = EVE_GAME_DIR "/ApartmentLifeDemo";
    if (argc > 1) {
        data_root = argv[1];
    }
    if (argc > 2) {
        game_root = argv[2];
    }

    eve::web_viewer::WebCharacterViewerApp app;
    if (!app.initialize(data_root, game_root)) {
        EVE_LOG(Error, "WebViewerApp", "Failed to initialize");
        return EXIT_FAILURE;
    }

    if (!app.run_vertical_slice_flow()) {
        EVE_LOG(Error, "WebViewerApp", "Vertical slice flow failed");
        app.shutdown();
        return EXIT_FAILURE;
    }

    app.shutdown();
    EVE_LOG(Info, "WebViewerApp", "Web Apartment Character Viewer completed successfully");
    return EXIT_SUCCESS;
}
