#include <eve/product/bug_triage.hpp>
#include <eve/product/content_completion.hpp>
#include <eve/product/project_archive_enhanced.hpp>
#include <eve/product/personal_product_cut.hpp>
#include <eve/product/release_bundle.hpp>
#include <eve/product/release_checklist.hpp>
#include <eve/product/save_stress_tester.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr const char* kDataRoot = EVE_DATA_DIR;
constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";
constexpr const char* kReleasesRoot = EVE_RELEASES_DIR;

} // namespace

TEST(ContentCompletionTest, MeetsApartmentLifeRequirements) {
    const eve::product::ContentCompletionReport report =
        eve::product::ContentCompletionValidator().validate(kGameRoot, kDataRoot);
    EXPECT_TRUE(report.complete) << report.missing_requirements.size();
    EXPECT_GE(report.wardrobe_items, 20u);
    EXPECT_GE(report.hairstyles, 5u);
    EXPECT_GE(report.expressions, 10u);
    EXPECT_GE(report.rooms, 6u);
    EXPECT_GE(report.object_interactions, 50u);
    EXPECT_GE(report.dialogue_lines, 150u);
    EXPECT_GE(report.events, 10u);
    EXPECT_GE(report.camera_presets, 12u);
    EXPECT_GE(report.lighting_presets, 10u);
    EXPECT_GE(report.photo_presets, 8u);
}

TEST(BugTriageTest, BlocksP0AndP1) {
    eve::product::BugTriage triage;
    triage.add_bug({"p2_01", eve::product::BugPriority::P2, "Minor camera jitter",
                    "Use room preset", false, ""});
    EXPECT_TRUE(triage.evaluate().release_allowed);

    triage.add_bug({"p1_01", eve::product::BugPriority::P1, "Editor crash", "", false, ""});
    EXPECT_FALSE(triage.evaluate().release_allowed);
}

TEST(SaveStressTest, SurvivesHundredCycles) {
    eve::product::SaveStressConfig config;
    config.data_root = kDataRoot;
    config.game_root = kGameRoot;
    config.slot_count = 10;
    config.cycle_count = 100;
    const eve::product::SaveStressResult result = eve::product::SaveStressTester().run(config);
    EXPECT_TRUE(result.passed) << result.failures.size();
    EXPECT_EQ(result.slots_tested, 10u);
    EXPECT_EQ(result.cycles_completed, 100u);
}

TEST(ReleaseChecklistTest, RunsAllCategories) {
    eve::product::ReleaseChecklistConfig config;
    config.data_root = kDataRoot;
    config.game_root = kGameRoot;
    config.releases_root = kReleasesRoot;
    const eve::product::ReleaseChecklistReport report =
        eve::product::ReleaseChecklist().run_all(config);
    EXPECT_GE(report.items.size(), 20u);
    EXPECT_GT(report.passed_count, 0u);
}

TEST(ProjectArchiveEnhancedTest, ExportValidateRestore) {
    const std::filesystem::path archive =
        std::filesystem::temp_directory_path() / "eve_product_archive";
    const std::filesystem::path restored =
        std::filesystem::temp_directory_path() / "eve_product_restored";
    eve::product::ArchiveOptions opts;
    opts.exclude_build_output = true;
    const auto exported = eve::product::ProjectArchiveEnhanced().export_archive(
        kGameRoot, archive.string(), opts);
    ASSERT_TRUE(exported.success);
    const auto validation =
        eve::product::ProjectArchiveEnhanced().validate_archive(archive.string());
    EXPECT_TRUE(validation.valid);
    EXPECT_TRUE(eve::product::ProjectArchiveEnhanced().restore_archive(archive.string(),
                                                                     restored.string()));
    std::filesystem::remove_all(archive);
    std::filesystem::remove_all(restored);
}

TEST(ReleaseBundleTest, CreatesRC1Artifacts) {
    eve::product::ReleaseBundleConfig config;
    config.data_root = kDataRoot;
    config.game_root = kGameRoot;
    config.output_root = kReleasesRoot;
    const eve::product::ReleaseBundleResult result =
        eve::product::ReleaseBundle().create(config);
    EXPECT_FALSE(result.bundle_root.empty());
    EXPECT_TRUE(std::filesystem::exists(result.bundle_root + "/TestReport.md"));
    EXPECT_TRUE(std::filesystem::exists(result.bundle_root + "/ValidationReport.md"));
    EXPECT_TRUE(std::filesystem::exists(result.bundle_root + "/ExportInstructions.md"));
}

TEST(PersonalProductCutTest, FullCutPipeline) {
    eve::product::PersonalProductConfig config;
    config.data_root = kDataRoot;
    config.game_root = kGameRoot;
    config.releases_root = kReleasesRoot;
    const eve::product::PersonalProductReport report =
        eve::product::PersonalProductCut().run_full_cut(config);
    EXPECT_TRUE(report.content.complete);
    EXPECT_TRUE(report.save_stress.passed);
    EXPECT_TRUE(report.bug_triage.release_allowed);
}
