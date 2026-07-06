#pragma once

#include <eve/interaction/input_types.hpp>
#include <eve/platform/mobile_input.hpp>

namespace eve::interaction {

class InputController {
public:
    void set_mode(InputMode mode) { mode_ = mode; }
    [[nodiscard]] InputMode mode() const { return mode_; }

    void on_mouse_move(f32 x, f32 y);
    void on_mouse_button(MouseButton button, bool pressed);
    void on_mouse_scroll(f32 delta);
    void on_mouse_double_click(f32 x, f32 y);
    void on_key_shift(bool pressed) { shift_held_ = pressed; }

    void on_touch_begin(f32 x, f32 y);
    void on_touch_move(f32 x, f32 y);
    void on_touch_end();
    void on_pinch(f32 scale);
    void on_two_finger_pan(f32 dx, f32 dy);
    void on_touch_tap(f32 x, f32 y);
    void on_touch_double_tap(f32 x, f32 y);
    void on_touch_long_press(f32 x, f32 y);

    void on_gamepad_orbit(f32 yaw, f32 pitch);
    void on_gamepad_zoom(f32 delta);

    void begin_frame();
    [[nodiscard]] const InputFrame& frame() const { return frame_; }
    [[nodiscard]] const PointerEvent& last_pointer() const { return last_pointer_; }
    [[nodiscard]] platform::MobileInputController& touch() { return touch_; }

private:
    void accumulate_orbit(f32 yaw, f32 pitch);
    f32 sensitivity() const;

    InputMode mode_ = InputMode::MouseKeyboard;
    InputFrame frame_;
    PointerEvent last_pointer_;
    platform::MobileInputController touch_;
    math::Vec2 last_mouse_{};
    bool mouse_dragging_ = false;
    MouseButton drag_button_ = MouseButton::None;
    bool shift_held_ = false;
    u32 tap_count_ = 0;
};

} // namespace eve::interaction
