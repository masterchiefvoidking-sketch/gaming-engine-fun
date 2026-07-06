#include <eve/render/inspection_camera.hpp>
#include <gtest/gtest.h>

TEST(InspectionCameraTest, OrbitsAndProjects) {
    eve::render::InspectionCamera camera;
    camera.set_focus({0.0f, 1.5f, 0.0f});
    eve::render::OrbitCameraInput input{};
    input.orbit_delta_yaw = 0.1f;
    input.zoom_delta = 0.2f;
    camera.update(1.0f / 60.0f, input);
    const eve::math::Mat4 view = camera.view_matrix();
    const eve::math::Mat4 proj = camera.projection_matrix(16.0f / 9.0f);
    EXPECT_NE(view.m[0], 0.0f);
    EXPECT_NE(proj.m[0], 0.0f);
}

TEST(InspectionCameraTest, AppliesPresets) {
    eve::render::InspectionCamera camera;
    camera.add_preset({"closeup", "Close Up", {0.0f, 1.6f, 1.0f}, {0.0f, 1.55f, 0.0f}, 30.0f});
    camera.apply_preset("closeup");
    EXPECT_NEAR(camera.position().z, 1.0f, 0.01f);
}
