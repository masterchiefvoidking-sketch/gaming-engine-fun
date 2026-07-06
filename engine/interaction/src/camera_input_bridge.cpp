#include <eve/interaction/camera_input_bridge.hpp>

namespace eve::interaction {

void CameraInputBridge::apply(render::InspectionCamera& camera, f32 delta_seconds,
                              const InputFrame& input) {
    last_input_ = {};
    last_input_.orbit_delta_yaw = input.orbit_delta_yaw;
    last_input_.orbit_delta_pitch = input.orbit_delta_pitch;
    last_input_.zoom_delta = input.zoom_delta;
    last_input_.pan_delta = input.pan_delta;
    camera.update(delta_seconds, last_input_);
}

} // namespace eve::interaction
