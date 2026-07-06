#include <eve/human/character_profile.hpp>
#include <eve/human/photo_mode_bridge.hpp>
#include <eve/render/human_shader_pipeline.hpp>
#include <eve/render/inspection_camera.hpp>
#include <eve/render/photo_mode.hpp>
#include <filesystem>
#include <gtest/gtest.h>

TEST(PhotoModeTest, CapturesScreenshotMetadata) {
    eve::render::InspectionCamera camera;
    eve::render::HumanShaderPipeline pipeline;
    pipeline.initialize();
    eve::render::PhotoMode photo_mode(camera, pipeline);
    eve::human::CharacterCreator creator;

    photo_mode.enable(true);
    eve::human::apply_photo_expression_preset(creator, "smile");
    const std::string path = "photo_test.hdr";
    photo_mode.capture_screenshot(path);
    EXPECT_TRUE(std::filesystem::exists(path));
    EXPECT_NEAR(creator.morph("mouth_smile"), 0.8f, 0.01f);
    std::filesystem::remove(path);
}
