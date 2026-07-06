#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>

namespace eve::interaction {

enum class InputMode : u8 {
    MouseKeyboard = 0,
    Touchscreen,
    Gamepad,
};

enum class MouseButton : u8 {
  None = 0,
  Left,
  Right,
  Middle,
};

enum class PointerAction : u8 {
    None = 0,
    Select,
    Focus,
    ContextMenu,
    Cancel,
};

struct PointerEvent {
    math::Vec2 position;
    PointerAction action = PointerAction::None;
    bool consumed = false;
};

struct InputFrame {
    f32 orbit_delta_yaw = 0.0f;
    f32 orbit_delta_pitch = 0.0f;
    f32 zoom_delta = 0.0f;
    math::Vec2 pan_delta{};
    bool select_pressed = false;
    bool focus_pressed = false;
    bool context_menu_pressed = false;
    bool cancel_pressed = false;
    f32 sensitivity_scale = 1.0f;
};

} // namespace eve::interaction
