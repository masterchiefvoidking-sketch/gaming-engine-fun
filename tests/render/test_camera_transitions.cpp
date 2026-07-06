#include <eve/render/inspection_camera.hpp>
#include <gtest/gtest.h>

TEST(InspectionCameraTest, BodyRegionFocusTransitions) {
    eve::render::InspectionCamera camera;
    camera.focus_body_region({0.0f, 1.15f, 0.0f});
    EXPECT_EQ(camera.mode(), eve::render::CameraMode::BodyRegionFocus);

    const eve::math::Vec3 start_pos = camera.position();
    camera.transition_to({0.5f, 1.3f, 1.5f}, {0.0f, 1.15f, 0.0f}, 0.5f);
    camera.update(0.25f);
    const eve::math::Vec3 mid_pos = camera.position();
    EXPECT_NE(mid_pos.x, start_pos.x);

    camera.update(0.25f);
    EXPECT_NEAR(camera.focus().y, 1.15f, 0.01f);
}

TEST(InspectionCameraTest, ContentRatingLockout) {
    eve::render::InspectionCamera camera;
    camera.set_content_rating_lockout(eve::render::ContentRatingLockout::Mature);
    EXPECT_TRUE(camera.is_locked_out(eve::render::ContentRatingLockout::AdultsOnly));
    EXPECT_FALSE(camera.is_locked_out(eve::render::ContentRatingLockout::Mature));
}

TEST(InspectionCameraTest, WardrobeAndMirrorModesOrbit) {
    eve::render::InspectionCamera camera;
    camera.set_mode(eve::render::CameraMode::Wardrobe);
    eve::render::OrbitCameraInput input{};
    input.orbit_delta_yaw = 0.2f;
    camera.update(1.0f / 60.0f, input);
    EXPECT_NE(camera.view_matrix().m[0], 0.0f);

    camera.set_mode(eve::render::CameraMode::Mirror);
    camera.update(1.0f / 60.0f, input);
    EXPECT_NE(camera.view_matrix().m[0], 0.0f);
}
