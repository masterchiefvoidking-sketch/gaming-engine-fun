#include <eve/build/build_pipeline.hpp>

#include <eve/build/dependency_manifest.hpp>
#include <eve/build/personal_backup_tool.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/platform/web_publisher.hpp>

#include <nlohmann/json.hpp>

namespace eve::build {

BuildPipelineResult BuildPipeline::export_project(const BuildPipelineConfig& config) const {
    BuildPipelineResult result;
    const ExportProfileSettings profile = settings_for_profile(config.profile);

    result.validation = ContentValidator().validate(config.project_root, profile);
    if (!result.validation.passed) {
        result.message = "Validation failed";
        return result;
    }

    if (config.create_backup && !config.backup_root.empty()) {
        PersonalBackupTool().auto_backup(config.project_root, config.backup_root,
                                         config.project_name);
    }

    const std::string out_dir =
        config.output_root.empty()
            ? config.project_root + "/Build/" + profile_name(config.profile)
            : config.output_root + "/" + profile_name(config.profile);
    FileSystem::create_directories(out_dir);

    result.cook = AssetCooker().cook(config.project_root, out_dir, profile);
    if (!result.cook.success) {
        result.message = result.cook.message;
        return result;
    }

    const DependencyManifest manifest =
        DependencyManifestWriter().build_from_cook(config.project_name,
                                                   profile_name(config.profile), result.cook);
    DependencyManifestWriter().write(out_dir + "/dependency_manifest.json", manifest);

    nlohmann::json build_info;
    build_info["project"] = config.project_name;
    build_info["profile"] = profile_name(config.profile);
    build_info["platform"] = platform_name(profile.platform);
    build_info["renderer"] = profile.renderer;
    build_info["offline_only"] = profile.offline_only;
    build_info["save_location"] = profile.save_location;
    build_info["compress_assets"] = profile.compress_assets;
    build_info["screenshot_support"] = profile.screenshot_support;
    build_info["debug_console"] = profile.debug_console;
    build_info["progressive_loading"] = profile.progressive_loading;
    build_info["touch_controls"] = profile.touch_controls;
    build_info["cooked_assets"] = result.cook.assets.size();
    build_info["wasm"] = profile.platform == PlatformTarget::Web;
    build_info["performance_profiler"] =
        profile.platform == PlatformTarget::Windows && profile.debug_console;
    FileSystem::write_text_file(out_dir + "/build_info.json", build_info.dump(2));

    if (profile.platform == PlatformTarget::Web) {
        platform::WebDeployConfig web;
        web.project_name = config.project_name;
        web.output_dir = out_dir + "/web";
        web.compress_assets = profile.compress_assets;
        web.progressive_loading = profile.progressive_loading;
        web.mobile_friendly_controls = profile.touch_controls;
        web.renderer = profile.renderer;
        const platform::WebDeployResult web_result =
            platform::WebPublisher().publish(web, config.project_root);
        if (!web_result.success) {
            result.message = web_result.message;
            return result;
        }
    } else {
        nlohmann::json package;
        package["project"] = config.project_name;
        package["target"] = platform_name(profile.platform);
        package["renderer"] = profile.renderer;
        package["game_root"] = config.project_root;
        package["cooked_dir"] = result.cook.output_dir;
        package["profiler"] = profile.debug_console;
        package["screenshot_folder"] = config.project_root + "/Screenshots";
        FileSystem::write_text_file(out_dir + "/package.json", package.dump(2));
    }

    result.success = true;
    result.output_path = out_dir;
    result.message = "Export complete";
    return result;
}

ValidationReport BuildPipeline::validate_only(const BuildPipelineConfig& config) const {
    return ContentValidator().validate(config.project_root,
                                       settings_for_profile(config.profile));
}

} // namespace eve::build
