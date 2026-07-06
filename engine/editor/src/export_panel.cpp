#include <eve/editor/export_panel.hpp>

#include <eve/core/filesystem/filesystem.hpp>

namespace eve::editor {

void ExportPanel::set_project(std::string_view root, std::string_view name) {
    state_.project_root = std::string(root);
    state_.project_name = std::string(name);
}

bool ExportPanel::validate() {
    build::BuildPipelineConfig config;
    config.project_root = state_.project_root;
    config.project_name = state_.project_name;
    config.profile = state_.profile;
    last_validation_ = pipeline_.validate_only(config);
    state_.last_success = last_validation_.passed;
    state_.last_message =
        last_validation_.passed ? "Validation passed" : "Validation failed";
    return last_validation_.passed;
}

bool ExportPanel::cook_and_build() {
    if (!validate()) {
        return false;
    }
    build::BuildPipelineConfig config;
    config.project_root = state_.project_root;
    config.project_name = state_.project_name;
    config.profile = state_.profile;
    config.create_backup = state_.create_backup;
    config.backup_root = state_.project_root + "/Backups";
    const build::BuildPipelineResult result = pipeline_.export_project(config);
    state_.last_success = result.success;
    state_.last_output_path = result.output_path;
    state_.last_message = result.message;
  last_validation_ = result.validation;
    return result.success;
}

bool ExportPanel::open_output_folder() const {
    return FileSystem::is_directory(state_.last_output_path);
}

} // namespace eve::editor
