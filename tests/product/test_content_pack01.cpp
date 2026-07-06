#include <eve/build/content_validator.hpp>
#include <eve/build/export_profiles.hpp>
#include <eve/product/content_completion.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr const char* kDataRoot = EVE_DATA_DIR;
constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";
constexpr const char* kPackRoot = EVE_REPO_DIR "/ContentPack01";

} // namespace

TEST(ContentPack01Test, DeliverablesExist) {
    const std::filesystem::path pack(kPackRoot);
    EXPECT_TRUE(std::filesystem::exists(pack / "CharacterProfile.md"));
    EXPECT_TRUE(std::filesystem::exists(pack / "DialogueDatabase.json"));
    EXPECT_TRUE(std::filesystem::exists(pack / "EventDatabase.json"));
    EXPECT_TRUE(std::filesystem::exists(pack / "CameraPresets.json"));
    EXPECT_TRUE(std::filesystem::exists(pack / "LightingPresets.json"));
    EXPECT_TRUE(std::filesystem::exists(pack / "PhotoModePresets.json"));
}

TEST(ContentPack01Test, MeetsContentMinimums) {
    const eve::product::ContentCompletionReport report =
        eve::product::ContentCompletionValidator().validate(kGameRoot, kDataRoot);
    EXPECT_TRUE(report.complete);
    EXPECT_GE(report.dialogue_lines, 150u);
    EXPECT_GE(report.camera_presets, 12u);
    EXPECT_GE(report.photo_presets, 8u);
    EXPECT_GE(report.object_interactions, 50u);
}

TEST(ContentPack01Test, PassesExportValidation) {
    const auto profile =
        eve::build::settings_for_profile(eve::build::ExportProfile::WindowsDevelopment);
    const eve::build::ValidationReport validation =
        eve::build::ContentValidator().validate(kGameRoot, profile);
    EXPECT_TRUE(validation.passed) << validation.issues.size();
}
