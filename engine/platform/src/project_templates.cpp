#include <eve/platform/project_templates.hpp>
#include <eve/content/project_layout.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>
#include <nlohmann/json.hpp>

namespace eve::platform {

void ProjectTemplateLibrary::register_template(ProjectTemplate tmpl) {
    templates_.push_back(std::move(tmpl));
}

bool ProjectTemplateLibrary::load_from_directory(std::string_view path) {
    if (!FileSystem::is_directory(path)) {
        return false;
    }
    for (const std::string& file : FileSystem::list_directory(path)) {
        if (file.size() < 6 || file.substr(file.size() - 5) != ".json") {
            continue;
        }
        const std::string content =
            FileSystem::read_text_file(std::string(path) + "/" + file);
        if (content.empty()) {
            continue;
        }
        try {
            const auto json = nlohmann::json::parse(content);
            ProjectTemplate tmpl;
            tmpl.id = json.value("id", "");
            tmpl.name = json.value("name", "");
            tmpl.description = json.value("description", "");
            tmpl.source_path = json.value("source", "");
            if (json.contains("includes")) {
                for (const auto& inc : json["includes"]) {
                    tmpl.includes.push_back(inc.get<std::string>());
                }
            }
            register_template(std::move(tmpl));
        } catch (const nlohmann::json::exception&) {
        }
    }
    return !templates_.empty();
}

bool ProjectTemplateLibrary::instantiate(std::string_view template_id,
                                         std::string_view dest_path) const {
    const ProjectTemplate* tmpl = find(template_id);
    if (tmpl == nullptr || tmpl->source_path.empty()) {
        return false;
    }
    std::filesystem::path source = tmpl->source_path;
    if (!source.is_absolute() && !content_root_.empty()) {
        source = std::filesystem::path(content_root_) / source;
    }
    if (!FileSystem::is_directory(source.string())) {
        return false;
    }
    for (const auto& entry : std::filesystem::recursive_directory_iterator(source)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        const std::filesystem::path rel = std::filesystem::relative(entry.path(), source);
        const std::filesystem::path out = std::filesystem::path(dest_path) / rel;
        std::filesystem::create_directories(out.parent_path());
        std::filesystem::copy_file(entry.path(), out,
                                   std::filesystem::copy_options::overwrite_existing);
    }
    content::ProjectLayout layout;
    layout.initialize(dest_path);
    layout.create_default_structure();
    return true;
}

const ProjectTemplate* ProjectTemplateLibrary::find(std::string_view id) const {
    for (const ProjectTemplate& tmpl : templates_) {
        if (tmpl.id == id) {
            return &tmpl;
        }
    }
    return nullptr;
}

} // namespace eve::platform
