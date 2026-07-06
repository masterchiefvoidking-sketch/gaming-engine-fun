#include <eve/interaction/responsive_ui_layout.hpp>

namespace eve::interaction {

namespace {
PanelLayout make_panel(UIPanel panel, bool visible, f32 x, f32 y, f32 w, f32 h, f32 touch_min) {
    PanelLayout layout;
    layout.panel = panel;
    layout.visible = visible;
    layout.x = x;
    layout.y = y;
    layout.width = w;
    layout.height = h;
    layout.touch_target_min = touch_min;
    return layout;
}
} // namespace

void ResponsiveUILayout::set_viewport(f32 width, f32 height) {
    viewport_width = width;
    viewport_height = height;
    if (width < 768.0f) {
        apply_profile(UILayoutProfile::Phone);
    } else if (width < 1200.0f) {
        apply_profile(UILayoutProfile::Tablet);
    } else {
        apply_profile(UILayoutProfile::Desktop);
    }
}

void ResponsiveUILayout::apply_profile(UILayoutProfile new_profile) {
    profile = new_profile;
    switch (profile) {
    case UILayoutProfile::Phone:
        panels[0] = make_panel(UIPanel::Character, true, 8.0f, 8.0f, viewport_width - 16.0f, 120.0f, 48.0f);
        panels[1] = make_panel(UIPanel::Wardrobe, false, 8.0f, 140.0f, viewport_width - 16.0f,
                               viewport_height - 160.0f, 48.0f);
        panels[6] = make_panel(UIPanel::Settings, false, 8.0f, 140.0f, viewport_width - 16.0f,
                               viewport_height - 160.0f, 48.0f);
        break;
    case UILayoutProfile::Tablet:
        panels[0] = make_panel(UIPanel::Character, true, 16.0f, 16.0f, 280.0f, 200.0f, 44.0f);
        panels[1] = make_panel(UIPanel::Wardrobe, true, 16.0f, 230.0f, 280.0f, 360.0f, 44.0f);
        panels[6] = make_panel(UIPanel::Settings, false, viewport_width - 296.0f, 16.0f, 280.0f, 400.0f,
                               44.0f);
        break;
    case UILayoutProfile::Desktop:
    default:
        panels[0] = make_panel(UIPanel::Character, true, 24.0f, 24.0f, 320.0f, 220.0f, 36.0f);
        panels[1] = make_panel(UIPanel::Wardrobe, true, 24.0f, 260.0f, 320.0f, 420.0f, 36.0f);
        panels[2] = make_panel(UIPanel::Relationship, true, viewport_width - 344.0f, 24.0f, 320.0f, 220.0f,
                               36.0f);
        panels[3] = make_panel(UIPanel::Apartment, false, viewport_width - 344.0f, 260.0f, 320.0f, 220.0f,
                               36.0f);
        panels[4] = make_panel(UIPanel::Camera, false, viewport_width - 344.0f, 500.0f, 320.0f, 180.0f, 36.0f);
        panels[5] = make_panel(UIPanel::PhotoMode, false, 24.0f, viewport_height - 200.0f, 320.0f, 176.0f,
                               36.0f);
        panels[6] = make_panel(UIPanel::Settings, false, viewport_width - 344.0f, viewport_height - 200.0f,
                               320.0f, 176.0f, 36.0f);
        break;
    }
}

PanelLayout& ResponsiveUILayout::panel(UIPanel id) {
    return panels[static_cast<std::size_t>(id)];
}

const PanelLayout& ResponsiveUILayout::panel(UIPanel id) const {
    return panels[static_cast<std::size_t>(id)];
}

f32 ResponsiveUILayout::touch_target_size() const {
    switch (profile) {
    case UILayoutProfile::Phone:
        return 48.0f;
    case UILayoutProfile::Tablet:
        return 44.0f;
    default:
        return 36.0f;
    }
}

} // namespace eve::interaction
