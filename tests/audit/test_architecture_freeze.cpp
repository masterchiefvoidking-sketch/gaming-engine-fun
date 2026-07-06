#include <eve/audit/architecture_freeze.hpp>
#include <eve/audit/dependency_review.hpp>
#include <eve/audit/module_health.hpp>
#include <eve/audit/test_coverage.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <gtest/gtest.h>

namespace {

constexpr const char* kRepoRoot = EVE_REPO_DIR;
constexpr const char* kDataRoot = EVE_DATA_DIR;
constexpr const char* kGameRoot = EVE_GAME_DIR "/ApartmentLifeDemo";

} // namespace

TEST(ModuleHealthTest, AuditsAllEngineModules) {
    const auto modules = eve::audit::ModuleHealthAuditor().audit_all(kRepoRoot);
    EXPECT_GE(modules.size(), 18u);
    bool found_core = false;
    for (const auto& mod : modules) {
        if (mod.name == "core") {
            found_core = true;
            EXPECT_GT(mod.lines_of_code, 0u);
            EXPECT_FALSE(mod.public_apis.empty());
        }
    }
    EXPECT_TRUE(found_core);
}

TEST(DependencyReviewTest, ListsMinimalDependencies) {
    const auto deps = eve::audit::DependencyReviewer().audit();
    EXPECT_EQ(deps.size(), 4u);
}

TEST(TestCoverageTest, ProducesCoverageEntries) {
    const auto coverage = eve::audit::TestCoverageAuditor().audit(kRepoRoot);
    EXPECT_GE(coverage.size(), 18u);
}

TEST(ArchitectureFreezeTest, WritesAllDeliverables) {
    eve::audit::ArchitectureFreezeConfig config;
    config.repo_root = kRepoRoot;
    config.data_root = kDataRoot;
    config.game_root = kGameRoot;
    config.output_dir = std::string(kRepoRoot) + "/build/_audit_test_output";
    const eve::audit::ArchitectureFreezeReport report = eve::audit::ArchitectureFreeze().run(config);
    EXPECT_TRUE(report.success);
    EXPECT_GE(report.modules.size(), 18u);
    EXPECT_GE(report.performance.size(), 8u);
    EXPECT_TRUE(eve::audit::ArchitectureFreeze().write_deliverables(config, report));
    EXPECT_TRUE(eve::FileSystem::exists(config.output_dir + "/ArchitectureReport.md"));
    EXPECT_TRUE(eve::FileSystem::exists(config.output_dir + "/PerformanceReport.md"));
    EXPECT_TRUE(eve::FileSystem::exists(config.output_dir + "/ReleaseReadinessAssessment.md"));
}
