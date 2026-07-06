#pragma once

#include <eve/build/build_pipeline.hpp>

#include <string>

namespace eve::editor {

struct ExportPanelState {
    std::string project_root;
    std::string project_name = "Apartment Life Demo";
    build::ExportProfile profile = build::ExportProfile::WindowsDevelopment;
    bool create_backup = true;
    std::string last_output_path;
    std::string last_message;
    bool last_success = false;
};

class ExportPanel {
public:
    void set_project(std::string_view root, std::string_view name);
    void set_profile(build::ExportProfile profile) { state_.profile = profile; }
    bool validate();
    bool cook_and_build();
    bool open_output_folder() const;
    bool run_build_locally() const;
    [[nodiscard]] const ExportPanelState& state() const { return state_; }
    [[nodiscard]] const build::ValidationReport& last_validation() const { return last_validation_; }

private:
    ExportPanelState state_;
    build::ValidationReport last_validation_;
    build::BuildPipeline pipeline_;
};

} // namespace eve::editor
