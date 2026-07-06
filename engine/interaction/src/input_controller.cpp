#include <eve/interaction/input_controller.hpp>

#include <cmath>

namespace eve::interaction {

namespace {
constexpr f32 kFineScale = 0.35f;
constexpr f32 kOrbitScale = 0.01f;
constexpr f32 kPanScale = 0.005f;
constexpr f32 kZoomScale = 0.15f;
} // namespace

void InputController::begin_frame() {
    frame_ = {};
}

f32 InputController::sensitivity() const {
    return shift_held_ ? kFineScale : 1.0f;
}

void InputController::accumulate_orbit(f32 yaw, f32 pitch) {
    const f32 scale = sensitivity();
    frame_.orbit_delta_yaw += yaw * scale;
    frame_.orbit_delta_pitch += pitch * scale;
}

void InputController::on_mouse_move(f32 x, f32 y) {
    if (!mouse_dragging_) {
        last_mouse_ = {x, y};
        return;
    }
    const math::Vec2 current{x, y};
    const math::Vec2 delta = current - last_mouse_;
    last_mouse_ = current;

    if (drag_button_ == MouseButton::Right) {
        accumulate_orbit(delta.x * kOrbitScale, delta.y * kOrbitScale);
    } else if (drag_button_ == MouseButton::Middle) {
        frame_.pan_delta = frame_.pan_delta + delta * kPanScale * sensitivity();
    }
}

void InputController::on_mouse_button(MouseButton button, bool pressed) {
    if (pressed) {
        drag_button_ = button;
        mouse_dragging_ = button == MouseButton::Right || button == MouseButton::Middle;
        if (button == MouseButton::Left) {
            frame_.select_pressed = true;
            last_pointer_.position = last_mouse_;
            last_pointer_.action = PointerAction::Select;
        }
    } else {
        if (drag_button_ == button) {
            mouse_dragging_ = false;
            drag_button_ = MouseButton::None;
        }
    }
}

void InputController::on_mouse_scroll(f32 delta) {
    frame_.zoom_delta += delta * kZoomScale * sensitivity();
}

void InputController::on_mouse_double_click(f32 x, f32 y) {
    last_pointer_ = {{x, y}, PointerAction::Focus};
    frame_.focus_pressed = true;
}

void InputController::on_touch_begin(f32 x, f32 y) {
    mode_ = InputMode::Touchscreen;
    touch_.on_touch_begin(x, y);
    last_mouse_ = {x, y};
}

void InputController::on_touch_move(f32 x, f32 y) {
    touch_.on_touch_move(x, y);
    const auto& state = touch_.state();
    if (state.gesture == platform::TouchGesture::Pan) {
        accumulate_orbit(state.delta.x * kOrbitScale, state.delta.y * kOrbitScale);
    }
}

void InputController::on_touch_end() {
    touch_.on_touch_end();
}

void InputController::on_pinch(f32 scale) {
    touch_.on_pinch(scale);
    frame_.zoom_delta += (1.0f - scale) * 0.5f;
}

void InputController::on_two_finger_pan(f32 dx, f32 dy) {
    frame_.pan_delta = frame_.pan_delta + math::Vec2{dx, dy} * kPanScale;
}

void InputController::on_touch_tap(f32 x, f32 y) {
    last_pointer_ = {{x, y}, PointerAction::Select};
    frame_.select_pressed = true;
}

void InputController::on_touch_double_tap(f32 x, f32 y) {
    last_pointer_ = {{x, y}, PointerAction::Focus};
    frame_.focus_pressed = true;
}

void InputController::on_touch_long_press(f32 x, f32 y) {
    last_pointer_ = {{x, y}, PointerAction::ContextMenu};
    frame_.context_menu_pressed = true;
}

void InputController::on_gamepad_orbit(f32 yaw, f32 pitch) {
    mode_ = InputMode::Gamepad;
    accumulate_orbit(yaw * kOrbitScale, pitch * kOrbitScale);
}

void InputController::on_gamepad_zoom(f32 delta) {
    mode_ = InputMode::Gamepad;
    frame_.zoom_delta += delta * kZoomScale;
}

} // namespace eve::interaction
