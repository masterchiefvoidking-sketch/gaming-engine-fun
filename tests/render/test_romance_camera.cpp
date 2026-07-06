#include <eve/render/inspection_camera.hpp>
#include <eve/render/romance_camera.hpp>
#include <gtest/gtest.h>

TEST(RomanceCameraTest, LoadsPresetsFromData) {
    eve::render::InspectionCamera camera;
    eve::render::RomanceCameraController romance;
    romance.attach(camera);
    romance.load_presets(EVE_DATA_DIR "/anime/romance_cameras.json");
    EXPECT_TRUE(romance.apply_preset("face_closeup"));
    EXPECT_TRUE(romance.composition_guide().enabled);
}

TEST(RomanceCameraTest, FaceCloseUpEnablesDepthOfField) {
    eve::render::InspectionCamera camera;
    eve::render::RomanceCameraController romance;
    romance.attach(camera);
    romance.load_presets(EVE_DATA_DIR "/anime/romance_cameras.json");
    romance.apply_preset("face_closeup");
    EXPECT_TRUE(camera.depth_of_field().enabled);
}

TEST(RomanceCameraTest, TransitionToPreset) {
    eve::render::InspectionCamera camera;
    eve::render::RomanceCameraController romance;
    romance.attach(camera);
    romance.load_presets(EVE_DATA_DIR "/anime/romance_cameras.json");
    romance.transition_to_preset("eye_closeup", 0.5f);
    EXPECT_NE(romance.active_preset(), nullptr);
}

TEST(RomanceCameraTest, FocalLengthAdjustsFov) {
    eve::render::InspectionCamera camera;
    eve::render::RomanceCameraController romance;
    romance.attach(camera);
    romance.set_focal_length(85.0f);
    EXPECT_LT(camera.focus().y, 2.0f);
}
