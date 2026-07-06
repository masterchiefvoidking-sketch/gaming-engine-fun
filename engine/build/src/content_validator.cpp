#include <eve/build/content_validator.hpp>

#include <eve/build/save_migration.hpp>
#include <eve/content/game_content_project.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>
#include <nlohmann/json.hpp>

namespace eve::build {

void ValidationReport::add_error(std::string code, std::string message, std::string path) {
    issues.push_back({ValidationSeverity::Error, std::move(code), std::move(message), std::move(path)});
}

void ValidationReport::add_warning(std::string code, std::string message, std::string path) {
    issues.push_back(
        {ValidationSeverity::Warning, std::move(code), std::move(message), std::move(path)});
}

bool ValidationReport::has_errors() const {
    for (const ValidationIssue& issue : issues) {
        if (issue.severity == ValidationSeverity::Error) {
            return true;
        }
    }
    return false;
}

namespace {

std::string resolve_asset_path(std::string_view project_root, const std::string& path) {
    if (FileSystem::exists(path)) {
        return path;
    }
    const std::string joined = std::string(project_root) + "/" + path;
    if (FileSystem::exists(joined)) {
        return joined;
    }
    const std::string marker = "/Assets/";
    const std::size_t pos = path.find(marker);
    if (pos != std::string::npos) {
        const std::string rel = path.substr(pos + 1);
        const std::string assets_path = std::string(project_root) + "/" + rel;
        if (FileSystem::exists(assets_path)) {
            return assets_path;
        }
    }
    return path;
}

} // namespace

ValidationReport ContentValidator::validate(std::string_view project_root,
                                            const ExportProfileSettings& profile) const {
    ValidationReport report;
    const std::string root(project_root);

    if (!FileSystem::exists(root + "/project.json")) {
        report.add_error("missing_project", "project.json not found", root + "/project.json");
        return report;
    }

    if (!FileSystem::is_directory(root + "/Assets")) {
        report.add_error("missing_assets", "Assets folder not found", root + "/Assets");
        return report;
    }

    content::GameContentProject project;
    if (!project.open(project_root)) {
        report.add_error("project_load_failed", "Failed to load game content project", root);
        return report;
    }

    if (project.characters().character_count() == 0) {
        report.add_warning("no_characters", "No characters defined");
    }

    const std::string index_path = root + "/Assets/content_index.json";
    if (FileSystem::exists(index_path)) {
        try {
            const auto json = nlohmann::json::parse(FileSystem::read_text_file(index_path));
            for (const auto& entry : json) {
                const std::string path = entry.value("path", "");
                if (!path.empty()) {
                    const std::string resolved = resolve_asset_path(project_root, path);
                    if (!FileSystem::exists(resolved)) {
                        report.add_error("broken_reference", "Indexed asset missing", path);
                    }
                }
            }
        } catch (const nlohmann::json::exception&) {
            report.add_error("invalid_index", "content_index.json is invalid", index_path);
        }
    }

    for (const std::string& file : FileSystem::list_directory(root + "/Assets/Dialogue")) {
        if (file.size() < 6 || file.substr(file.size() - 5) != ".json") {
            continue;
        }
        const std::string path = root + "/Assets/Dialogue/" + file;
        try {
            const auto json = nlohmann::json::parse(FileSystem::read_text_file(path));
            if (!json.contains("nodes") && !json.contains("id")) {
                report.add_error("dialogue_graph_error", "Invalid dialogue package", path);
            }
        } catch (const nlohmann::json::exception&) {
            report.add_error("dialogue_graph_error", "Dialogue JSON parse failed", path);
        }
    }

    for (const std::string& file : FileSystem::list_directory(root + "/Assets/Events")) {
        if (file.size() < 6 || file.substr(file.size() - 5) != ".json") {
            continue;
        }
        const std::string path = root + "/Assets/Events/" + file;
        try {
            const auto json = nlohmann::json::parse(FileSystem::read_text_file(path));
            if (!json.contains("nodes")) {
                report.add_warning("event_graph_warning", "Event missing nodes array", path);
            }
        } catch (const nlohmann::json::exception&) {
            report.add_error("event_graph_error", "Event JSON parse failed", path);
        }
    }

    if (profile.platform == PlatformTarget::Web && profile.renderer == "vulkan") {
        report.add_error("platform_incompatible", "Vulkan renderer is not supported on Web");
    }
    if (profile.platform == PlatformTarget::Windows && profile.renderer == "webgpu") {
        report.add_warning("platform_renderer", "WebGPU is unusual for Windows desktop builds");
    }

    for (const std::string& file : FileSystem::list_directory(root + "/Assets/Characters/Wardrobe")) {
        if (file.size() < 6 || file.substr(file.size() - 5) != ".json") {
            continue;
        }
        const std::string path = root + "/Assets/Characters/Wardrobe/" + file;
        try {
            const auto json = nlohmann::json::parse(FileSystem::read_text_file(path));
            if (!json.contains("id") || !json.contains("category")) {
                report.add_error("clothing_compatibility", "Wardrobe item missing required fields",
                                 path);
            }
            if (json.contains("material") && json["material"].get<std::string>().empty()) {
                report.add_warning("missing_shader", "Wardrobe item missing material shader", path);
            }
        } catch (const nlohmann::json::exception&) {
            report.add_error("clothing_compatibility", "Invalid wardrobe item", path);
        }
    }

    const std::string animations_dir = root + "/Assets/Animations";
    if (FileSystem::is_directory(animations_dir)) {
        bool has_animation = false;
        for (const std::string& file : FileSystem::list_directory(animations_dir)) {
            if (file.ends_with(".json") || file.ends_with(".glb")) {
                has_animation = true;
            }
        }
        if (!has_animation) {
            report.add_warning("no_animations", "Animations folder has no clip files");
        }
    }

    const std::string saves_dir = root + "/Saves";
    if (FileSystem::is_directory(saves_dir)) {
        for (const std::string& file : FileSystem::list_directory(saves_dir)) {
            if (!file.ends_with(".json")) {
                continue;
            }
            const std::string path = saves_dir + "/" + file;
            if (SaveMigration().needs_migration(path)) {
                report.add_warning("save_schema_mismatch", "Save file needs migration", path);
            }
        }
    }

    report.passed = !report.has_errors();
    return report;
}

} // namespace eve::build
