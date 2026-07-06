#pragma once

#include <string>
#include <vector>

namespace eve::content {

struct ProjectPaths {
    std::string root;
    std::string assets;
    std::string characters;
    std::string apartments;
    std::string furniture;
    std::string animations;
    std::string dialogue;
    std::string events;
    std::string localization;
    std::string saves;
    std::string mods;
};

class ProjectLayout {
public:
    bool initialize(std::string_view game_root);
    bool create_default_structure() const;
    [[nodiscard]] const ProjectPaths& paths() const { return paths_; }
    [[nodiscard]] const std::string& title() const { return title_; }
    void set_title(std::string_view title) { title_ = std::string(title); }

    [[nodiscard]] std::vector<std::string> standard_folders() const;

private:
    ProjectPaths paths_;
    std::string title_ = "Untitled Project";
};

} // namespace eve::content
