#pragma once

#include <eve/build/asset_cooker.hpp>
#include <eve/build/content_validator.hpp>
#include <eve/build/export_profiles.hpp>

#include <string>

namespace eve::build {

struct BuildPipelineConfig {
    std::string project_root;
    std::string project_name;
    std::string output_root;
    ExportProfile profile = ExportProfile::WindowsDevelopment;
    bool create_backup = true;
    std::string backup_root;
};

struct BuildPipelineResult {
    bool success = false;
    std::string output_path;
    ValidationReport validation;
    AssetCookResult cook;
    std::string message;
};

class BuildPipeline {
public:
    BuildPipelineResult export_project(const BuildPipelineConfig& config) const;
    [[nodiscard]] ValidationReport validate_only(const BuildPipelineConfig& config) const;
};

} // namespace eve::build
