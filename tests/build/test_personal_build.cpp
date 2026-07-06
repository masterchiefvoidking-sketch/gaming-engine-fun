#include <eve/build/asset_cooker.hpp>
#include <eve/build/build_pipeline.hpp>
#include <eve/build/content_validator.hpp>
#include <eve/build/dependency_manifest.hpp>
#include <eve/build/export_profiles.hpp>
#include <eve/build/personal_backup_tool.hpp>
#include <eve/build/personal_content_pack.hpp>
#include <eve/build/project_archive.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/editor/export_panel.hpp>
#include <eve/personal_export/personal_export_demo.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";
constexpr const char* kDataRoot = EVE_DATA_DIR;

} // namespace

TEST(ExportProfileTest, WindowsAndWebProfiles) {
    const auto windows =
        eve::build::settings_for_profile(eve::build::ExportProfile::WindowsDevelopment);
    EXPECT_EQ(windows.renderer, "vulkan");
    EXPECT_EQ(windows.save_location, "local_files");
    EXPECT_TRUE(windows.debug_console);

    const auto web = eve::build::settings_for_profile(eve::build::ExportProfile::WebRelease);
    EXPECT_EQ(web.renderer, "webgpu");
    EXPECT_EQ(web.save_location, "indexeddb");
    EXPECT_TRUE(web.touch_controls);
    EXPECT_TRUE(web.compress_assets);
}

TEST(AssetCookerTest, CooksRuntimePackage) {
    const std::filesystem::path out =
        std::filesystem::temp_directory_path() / "eve_cook_test";
    const auto profile =
        eve::build::settings_for_profile(eve::build::ExportProfile::WindowsDevelopment);
    const eve::build::AssetCookResult result =
        eve::build::AssetCooker().cook(kGameRoot, out.string(), profile);
    ASSERT_TRUE(result.success);
    EXPECT_GT(result.assets.size(), 10u);
    EXPECT_TRUE(eve::FileSystem::exists(result.output_dir + "/runtime_package.json"));
    std::filesystem::remove_all(out);
}

TEST(ContentValidatorTest, PassesApartmentLifeDemo) {
    const auto profile =
        eve::build::settings_for_profile(eve::build::ExportProfile::WindowsDevelopment);
    const eve::build::ValidationReport report =
        eve::build::ContentValidator().validate(kGameRoot, profile);
    EXPECT_TRUE(report.passed) << report.issues.size();
}

TEST(ContentValidatorTest, DetectsMissingProject) {
    const auto profile =
        eve::build::settings_for_profile(eve::build::ExportProfile::WebDevelopment);
    const eve::build::ValidationReport report = eve::build::ContentValidator().validate(
        "/tmp/nonexistent_eve_project", profile);
    EXPECT_FALSE(report.passed);
    EXPECT_TRUE(report.has_errors());
}

TEST(DependencyManifestTest, WritesManifest) {
    const auto profile =
        eve::build::settings_for_profile(eve::build::ExportProfile::WebDevelopment);
    const eve::build::AssetCookResult cook =
        eve::build::AssetCooker().cook(kGameRoot, std::filesystem::temp_directory_path().string(),
                                       profile);
    const eve::build::DependencyManifest manifest =
        eve::build::DependencyManifestWriter().build_from_cook("apartment_life", "web_development",
                                                                 cook);
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / "eve_dep_manifest.json";
    EXPECT_TRUE(eve::build::DependencyManifestWriter().write(path.string(), manifest));
    EXPECT_GT(manifest.entries.size(), 0u);
    std::filesystem::remove(path);
}

TEST(ProjectArchiveTest, ExportImportRoundTrip) {
    const std::filesystem::path archive =
        std::filesystem::temp_directory_path() / "eve_archive_export";
    const std::filesystem::path restored =
        std::filesystem::temp_directory_path() / "eve_archive_import";
    const eve::build::ArchiveResult exported =
        eve::build::ProjectArchive().export_archive(kGameRoot, archive.string());
    ASSERT_TRUE(exported.success);
    const eve::build::ArchiveResult imported =
        eve::build::ProjectArchive().import_archive(archive.string(), restored.string());
    ASSERT_TRUE(imported.success);
    EXPECT_TRUE(eve::FileSystem::exists((restored / "project.json").string()));
    std::filesystem::remove_all(archive);
    std::filesystem::remove_all(restored);
}

TEST(PersonalContentPackTest, ExportImportPack) {
    const std::filesystem::path pack_out =
        std::filesystem::temp_directory_path() / "eve_content_pack";
    const std::filesystem::path dest =
        std::filesystem::temp_directory_path() / "eve_pack_import";
    eve::build::ContentPackManifest manifest;
    manifest.id = "mira_pack";
    manifest.name = "Mira Content Pack";
    manifest.includes = {"Characters", "Dialogue/mira_greetings.json"};
    const eve::build::ContentPackResult exported = eve::build::PersonalContentPack().export_pack(
        std::string(kGameRoot) + "/Assets", pack_out.string(), manifest);
    ASSERT_TRUE(exported.success);
    const eve::build::ContentPackResult imported =
        eve::build::PersonalContentPack().import_pack(pack_out.string(), dest.string());
    EXPECT_TRUE(imported.success);
    std::filesystem::remove_all(pack_out);
    std::filesystem::remove_all(dest);
}

TEST(PersonalBackupToolTest, AutoBackupAndList) {
    const std::filesystem::path backup_root =
        std::filesystem::temp_directory_path() / "eve_personal_backup";
    eve::build::PersonalBackupTool tool;
    const eve::build::BackupToolResult backup =
        tool.auto_backup(kGameRoot, backup_root.string(), "apartment_life");
    ASSERT_TRUE(backup.success);
    EXPECT_FALSE(tool.list(backup_root.string()).empty());
    std::filesystem::remove_all(backup_root);
}

TEST(BuildPipelineTest, WindowsDevelopmentExport) {
    const std::filesystem::path out =
        std::filesystem::temp_directory_path() / "eve_windows_export";
    eve::build::BuildPipelineConfig config;
    config.project_root = kGameRoot;
    config.project_name = "Apartment Life Demo";
    config.profile = eve::build::ExportProfile::WindowsDevelopment;
    config.output_root = out.string();
    config.create_backup = false;
    const eve::build::BuildPipelineResult result =
        eve::build::BuildPipeline().export_project(config);
    ASSERT_TRUE(result.success) << result.message;
    EXPECT_TRUE(eve::FileSystem::exists(result.output_path + "/build_info.json"));
    EXPECT_TRUE(eve::FileSystem::exists(result.output_path + "/dependency_manifest.json"));
    std::filesystem::remove_all(out);
}

TEST(BuildPipelineTest, WebDevelopmentExport) {
    const std::filesystem::path out = std::filesystem::temp_directory_path() / "eve_web_export";
    eve::build::BuildPipelineConfig config;
    config.project_root = kGameRoot;
    config.project_name = "Apartment Life Demo";
    config.profile = eve::build::ExportProfile::WebDevelopment;
    config.output_root = out.string();
    config.create_backup = false;
    const eve::build::BuildPipelineResult result =
        eve::build::BuildPipeline().export_project(config);
    ASSERT_TRUE(result.success) << result.message;
    EXPECT_TRUE(eve::FileSystem::exists(result.output_path + "/web/manifest.json"));
    std::filesystem::remove_all(out);
}

TEST(ExportPanelTest, ValidatesAndBuilds) {
    eve::editor::ExportPanel panel;
    panel.set_project(kGameRoot, "Apartment Life Demo");
    panel.set_profile(eve::build::ExportProfile::WindowsDevelopment);
    EXPECT_TRUE(panel.validate());
    const std::filesystem::path original_build = std::filesystem::path(kGameRoot) / "Build";
    EXPECT_TRUE(panel.cook_and_build());
    EXPECT_TRUE(panel.open_output_folder());
}

TEST(PersonalExportDemoTest, ExportsWindowsAndWeb) {
    const std::filesystem::path backup = std::filesystem::path(kGameRoot) / "Backups";
    eve::personal_export::PersonalExportDemo demo;
    const eve::personal_export::PersonalExportDemoResult result =
        demo.run(kGameRoot, "Apartment Life Demo");
    ASSERT_TRUE(result.success) << result.message;
    EXPECT_FALSE(result.windows_output.empty());
    EXPECT_FALSE(result.web_output.empty());
    if (std::filesystem::exists(backup)) {
        std::filesystem::remove_all(backup);
    }
}
