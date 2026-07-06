#include <eve/web_viewer/web_character_viewer_app.hpp>

#include <eve/core/logging/logger.hpp>

namespace eve::web_viewer {

bool WebCharacterViewerApp::initialize(std::string_view data_root, std::string_view game_root) {
    interaction::WebCharacterViewerConfig config;
    config.data_root = std::string(data_root);
    config.game_root = std::string(game_root);
    config.project_id = "apartment_life";
    config.offline_only = true;
    config.viewport_width = 1280.0f;
    config.viewport_height = 720.0f;
    if (!viewer_.initialize(config)) {
        return false;
    }
    viewer_.apply_web_profile();
    EVE_LOG(Info, "WebViewerApp", "Web Apartment Character Viewer ready");
    return true;
}

void WebCharacterViewerApp::shutdown() {
    viewer_.shutdown();
}

void WebCharacterViewerApp::update(f32 delta_seconds) {
    viewer_.update(delta_seconds);
}

bool WebCharacterViewerApp::run_vertical_slice_flow() {
    auto& input = viewer_.input();
    input.on_mouse_button(interaction::MouseButton::Right, true);
    input.on_mouse_move(120.0f, 40.0f);
    input.on_mouse_button(interaction::MouseButton::Right, false);
    input.on_mouse_scroll(-1.0f);
    viewer_.update(0.016f);

    if (!viewer_.handle_select()) {
        return false;
    }
    if (!viewer_.open_interaction_ring()) {
        return false;
    }
    if (!viewer_.change_outfit("mira_outfit_01")) {
        return false;
    }
    if (!viewer_.change_hair("mira_hair_bun")) {
        return false;
    }
    if (!viewer_.change_expression("soft_smile")) {
        return false;
    }
    if (!viewer_.execute_ring_action(interaction::RingActionType::Talk)) {
        return false;
    }
    if (!viewer_.execute_ring_action(interaction::RingActionType::Compliment)) {
        return false;
    }
    if (!viewer_.enter_photo_mode(true)) {
        return false;
    }
    if (!viewer_.save_session()) {
        return false;
    }
    if (!viewer_.load_session()) {
        return false;
    }
    return true;
}

} // namespace eve::web_viewer
