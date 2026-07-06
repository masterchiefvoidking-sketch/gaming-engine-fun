#include <eve/platform/mobile_input.hpp>

namespace eve::platform {

void MobileInputController::on_touch_begin(f32 x, f32 y) {
    state_.active = true;
    state_.position = {x, y};
    last_position_ = state_.position;
    state_.gesture = TouchGesture::Tap;
}

void MobileInputController::on_touch_move(f32 x, f32 y) {
    if (!state_.active) {
        return;
    }
    const math::Vec2 current{x, y};
    state_.delta = current - last_position_;
    last_position_ = current;
    state_.position = current;
    if (state_.delta.dot(state_.delta) > 0.001f) {
        state_.gesture = TouchGesture::Pan;
    }
}

void MobileInputController::on_touch_end() {
    state_.active = false;
    state_.gesture = TouchGesture::None;
}

void MobileInputController::on_pinch(f32 scale) {
    state_.pinch_scale = scale;
    state_.gesture = TouchGesture::PinchZoom;
}

void MobileInputController::apply_orbit_camera(f32& yaw, f32& pitch, f32& zoom) {
    if (state_.gesture == TouchGesture::Pan) {
        yaw += state_.delta.x * 0.01f;
        pitch += state_.delta.y * 0.01f;
        state_.gesture = TouchGesture::Orbit;
    }
    if (state_.gesture == TouchGesture::PinchZoom) {
        zoom *= state_.pinch_scale;
    }
}

} // namespace eve::platform
