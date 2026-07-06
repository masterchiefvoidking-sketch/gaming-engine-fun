#pragma once

#include <eve/interaction/web_character_viewer.hpp>

namespace eve::web_viewer {

class WebCharacterViewerApp {
public:
    bool initialize(std::string_view data_root, std::string_view game_root);
    void shutdown();
    void update(f32 delta_seconds);
    bool run_vertical_slice_flow();

    [[nodiscard]] interaction::WebCharacterViewer& viewer() { return viewer_; }

private:
    interaction::WebCharacterViewer viewer_;
};

} // namespace eve::web_viewer
