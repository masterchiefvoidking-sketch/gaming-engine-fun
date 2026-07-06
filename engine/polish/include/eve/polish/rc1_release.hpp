#pragma once

#include <eve/polish/bug_tracker.hpp>
#include <eve/polish/performance_profiler.hpp>
#include <eve/polish/validation_gate.hpp>

#include <string>
#include <vector>

namespace eve::polish {

struct RC1BuildArtifact {
    std::string profile_name;
    std::string output_path;
    bool success = false;
};

struct RC1ReleaseReport {
    bool success = false;
    std::vector<RC1BuildArtifact> builds;
    ValidationGateResult validation;
    PerformanceSnapshot performance;
    std::string test_report_path;
    std::string asset_validation_path;
    std::string performance_report_path;
    std::string known_issues_path;
    std::string export_instructions_path;
    std::string next_phase_path;
    std::string message;
};

struct RC1ReleaseConfig {
    std::string data_root;
    std::string game_root;
    std::string output_root;
    std::string project_name = "apartment_life";
};

class RC1ReleasePipeline {
public:
    RC1ReleaseReport build_all(const RC1ReleaseConfig& config);
    bool generate_performance_report(std::string_view path, const PerformanceProfiler& profiler) const;
    bool generate_test_report(std::string_view path, u32 passed, u32 total) const;
    bool generate_export_instructions(std::string_view path) const;
    bool generate_next_phase_recommendations(std::string_view path) const;
};

} // namespace eve::polish
