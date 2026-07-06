#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::audit {

struct ModuleHealthReport {
    std::string name;
    std::string purpose;
    std::vector<std::string> public_apis;
    std::vector<std::string> dependencies;
    u32 lines_of_code = 0;
    u32 cyclomatic_complexity = 0;
    f32 test_coverage_percent = 0.0f;
    std::string performance_notes;
    std::vector<std::string> known_issues;
    std::vector<std::string> recommendations;
};

struct PerformanceMeasurement {
    std::string operation;
    f64 milliseconds = 0.0;
    std::string notes;
};

struct MemoryCategoryReport {
    std::string category;
    u64 estimated_bytes = 0;
    std::string notes;
};

struct TestCoverageEntry {
    std::string module;
    u32 test_count = 0;
    f32 coverage_percent = 0.0f;
    f32 target_percent = 0.0f;
    bool meets_target = false;
};

struct DependencyEntry {
    std::string name;
    std::string version;
    std::string purpose;
    bool required = true;
};

struct ApiReviewEntry {
    std::string module;
    std::string api;
    std::string issue;
    std::string recommendation;
};

struct TechnicalDebtEntry {
    std::string id;
    std::string severity;
    std::string description;
    std::string module;
    std::string remediation;
};

struct ArchitectureFreezeConfig {
    std::string repo_root;
    std::string data_root;
    std::string game_root;
    std::string output_dir = "Audit";
};

struct ArchitectureFreezeReport {
    bool success = false;
    std::vector<ModuleHealthReport> modules;
    std::vector<PerformanceMeasurement> performance;
    std::vector<MemoryCategoryReport> memory;
    std::vector<TestCoverageEntry> coverage;
    std::vector<DependencyEntry> dependencies;
    std::vector<ApiReviewEntry> api_issues;
    std::vector<TechnicalDebtEntry> technical_debt;
    std::vector<std::string> outstanding_issues;
    std::string release_readiness;
    std::string message;
};

} // namespace eve::audit
