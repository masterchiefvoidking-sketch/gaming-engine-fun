#include <eve/interaction/player_ui_state.hpp>

namespace eve::interaction {

void PlayerUIState::toggle_panel(UIPanel panel) {
    PanelLayout& p = layout_.panel(panel);
    p.visible = !p.visible;
}

void PlayerUIState::set_panel_visible(UIPanel panel, bool visible) {
    layout_.panel(panel).visible = visible;
}

void PlayerUIState::collapse_panel(UIPanel panel, bool collapsed) {
    layout_.panel(panel).collapsed = collapsed;
}

bool PlayerUIState::is_panel_visible(UIPanel panel) const {
    return layout_.panel(panel).visible;
}

} // namespace eve::interaction
