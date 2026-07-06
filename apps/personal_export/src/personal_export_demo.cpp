#include <eve/personal_export/personal_export_demo.hpp>

#include <eve/core/logging/logger.hpp>

namespace eve::personal_export {

PersonalExportDemoResult PersonalExportDemo::run(std::string_view project_root,
                                                 std::string_view project_name) {
    PersonalExportDemoResult result;
    build::BuildPipeline pipeline;

    build::BuildPipelineConfig windows;
    windows.project_root = std::string(project_root);
    windows.project_name = std::string(project_name);
    windows.profile = build::ExportProfile::WindowsDevelopment;
    windows.backup_root = windows.project_root + "/Backups";
    const build::BuildPipelineResult windows_result = pipeline.export_project(windows);
    if (!windows_result.success) {
        result.message = "Windows export failed: " + windows_result.message;
        return result;
    }
    result.windows_output = windows_result.output_path;

    build::BuildPipelineConfig web;
    web.project_root = windows.project_root;
    web.project_name = windows.project_name;
    web.profile = build::ExportProfile::WebDevelopment;
    web.backup_root = web.project_root + "/Backups";
    const build::BuildPipelineResult web_result = pipeline.export_project(web);
    if (!web_result.success) {
        result.message = "Web export failed: " + web_result.message;
        return result;
    }
    result.web_output = web_result.output_path;

    result.success = true;
    result.message = "Apartment Life Demo exported to Windows and Web";
    EVE_LOG(Info, "PersonalExport", result.message);
    return result;
}

} // namespace eve::personal_export
