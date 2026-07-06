#pragma once

#include <eve/core/types.hpp>

namespace eve::interaction {

enum class UILayoutProfile : u8 {
    Desktop = 0,
    Tablet,
    Phone,
};

enum class UIPanel : u8 {
    Character = 0,
    Wardrobe,
    Relationship,
    Apartment,
    Camera,
    PhotoMode,
    Settings,
};

struct PanelLayout {
    UIPanel panel = UIPanel::Character;
    bool visible = false;
    bool collapsed = false;
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 width = 320.0f;
    f32 height = 480.0f;
    f32 touch_target_min = 44.0f;
};

struct ResponsiveUILayout {
    UILayoutProfile profile = UILayoutProfile::Desktop;
    f32 viewport_width = 1920.0f;
    f32 viewport_height = 1080.0f;
    PanelLayout panels[7]{};

    void set_viewport(f32 width, f32 height);
    void apply_profile(UILayoutProfile profile);
    [[nodiscard]] PanelLayout& panel(UIPanel id);
    [[nodiscard]] const PanelLayout& panel(UIPanel id) const;
    [[nodiscard]] f32 touch_target_size() const;
};

} // namespace eve::interaction
