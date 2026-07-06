#pragma once

#include <string>
#include <vector>

namespace eve::platform {

struct ProjectTemplate {
    std::string id;
    std::string name;
    std::string description;
    std::string source_path;
    std::vector<std::string> includes;
};

class ProjectTemplateLibrary {
public:
    void set_content_root(std::string_view root) { content_root_ = std::string(root); }
    void register_template(ProjectTemplate tmpl);
    bool load_from_directory(std::string_view path);
    bool instantiate(std::string_view template_id, std::string_view dest_path) const;
    [[nodiscard]] const ProjectTemplate* find(std::string_view id) const;
    [[nodiscard]] std::size_t template_count() const { return templates_.size(); }

private:
    std::vector<ProjectTemplate> templates_;
    std::string content_root_;
};

} // namespace eve::platform
