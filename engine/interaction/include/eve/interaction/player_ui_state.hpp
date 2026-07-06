#pragma once

#include <eve/interaction/responsive_ui_layout.hpp>

namespace eve::interaction {

class PlayerUIState {
public:
    void toggle_panel(UIPanel panel);
    void set_panel_visible(UIPanel panel, bool visible);
    void collapse_panel(UIPanel panel, bool collapsed);
    [[nodiscard]] bool is_panel_visible(UIPanel panel) const;
    [[nodiscard]] ResponsiveUILayout& layout() { return layout_; }
    [[nodiscard]] const ResponsiveUILayout& layout() const { return layout_; }

private:
    ResponsiveUILayout layout_;
};

} // namespace eve::interaction
