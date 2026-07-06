#include <eve/content/packaging.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::content {

PackagingResult ProjectPackager::export_project(const PackagingConfig& config,
                                                std::string_view game_root) const {
    PackagingResult result;
    if (!FileSystem::exists(game_root)) {
        result.message = "Game root not found";
        return result;
    }

    const std::string out_base = config.output_dir.empty()
                                     ? std::string(game_root) + "/Build"
                                     : config.output_dir;
    FileSystem::create_directories(out_base);

    for (ExportTarget target : config.targets) {
        const std::string target_dir = out_base + "/" + target_name(target);
        FileSystem::create_directories(target_dir);

        nlohmann::json manifest;
        manifest["project"] = config.project_name;
        manifest["version"] = config.version;
        manifest["target"] = target_name(target);
        manifest["game_root"] = std::string(game_root);
        manifest["compressed"] = config.compress_assets;
        FileSystem::write_text_file(target_dir + "/package.json", manifest.dump(2));
    }

    result.success = true;
    result.output_path = out_base;
    result.message = "Export complete";
    return result;
}

std::string ProjectPackager::target_name(ExportTarget target) const {
    switch (target) {
    case ExportTarget::Linux:
        return "linux";
    case ExportTarget::SteamDeck:
        return "steamdeck";
    default:
        return "windows";
    }
}

} // namespace eve::content
