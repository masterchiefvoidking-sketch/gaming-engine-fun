#include <chrono>
#include <eve/render/inspection_camera.hpp>
#include <iostream>

int main() {
    eve::render::InspectionCamera camera;
    const auto start = std::chrono::steady_clock::now();
    constexpr int iterations = 100000;
    for (int i = 0; i < iterations; ++i) {
        eve::render::OrbitCameraInput input{};
        input.orbit_delta_yaw = 0.001f;
        input.zoom_delta = 0.0001f;
        camera.update(1.0f / 60.0f, input);
        (void)camera.view_matrix();
        (void)camera.projection_matrix(16.0f / 9.0f);
    }
    const auto end = std::chrono::steady_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Inspection camera benchmark: " << iterations << " iterations in " << ms
              << " ms\n";
    return 0;
}
