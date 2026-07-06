#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>

namespace eve::platform {

enum class TouchGesture : u8 {
    None = 0,
    Tap,
    DoubleTap,
    Pan,
    PinchZoom,
    Orbit,
};

struct TouchInputState {
    bool active = false;
    math::Vec2 position;
    math::Vec2 delta;
    f32 pinch_scale = 1.0f;
    TouchGesture gesture = TouchGesture::None;
};

struct MobileRenderSettings {
    u32 quality_level = 2;
    bool reduce_hair_physics = false;
    bool limit_shadows = true;
    f32 target_frame_time_ms = 16.67f;
    bool battery_saver = false;
};

class MobileInputController {
public:
    void on_touch_begin(f32 x, f32 y);
    void on_touch_move(f32 x, f32 y);
    void on_touch_end();
    void on_pinch(f32 scale);

    void apply_orbit_camera(f32& yaw, f32& pitch, f32& zoom);
    [[nodiscard]] const TouchInputState& state() const { return state_; }
    [[nodiscard]] MobileRenderSettings& render_settings() { return render_settings_; }

private:
    TouchInputState state_;
    math::Vec2 last_position_;
    MobileRenderSettings render_settings_;
};

} // namespace eve::platform
