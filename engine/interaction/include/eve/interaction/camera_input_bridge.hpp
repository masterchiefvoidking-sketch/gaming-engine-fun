#pragma once

#include <eve/interaction/input_types.hpp>
#include <eve/render/inspection_camera.hpp>

namespace eve::interaction {

class CameraInputBridge {
public:
    void apply(render::InspectionCamera& camera, f32 delta_seconds, const InputFrame& input);
    [[nodiscard]] const render::OrbitCameraInput& last_orbit_input() const { return last_input_; }

private:
    render::OrbitCameraInput last_input_{};
};

} // namespace eve::interaction
