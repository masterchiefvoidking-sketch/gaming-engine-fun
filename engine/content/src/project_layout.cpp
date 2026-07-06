#include <eve/content/project_layout.hpp>
#include <eve/core/filesystem/filesystem.hpp>

namespace eve::content {

bool ProjectLayout::initialize(std::string_view game_root) {
    paths_.root = std::string(game_root);
    paths_.assets = paths_.root + "/Assets";
    paths_.characters = paths_.assets + "/Characters";
    paths_.apartments = paths_.assets + "/Apartments";
    paths_.furniture = paths_.assets + "/Furniture";
    paths_.animations = paths_.assets + "/Animations";
    paths_.dialogue = paths_.assets + "/Dialogue";
    paths_.events = paths_.assets + "/Events";
    paths_.localization = paths_.assets + "/Localization";
    paths_.saves = paths_.root + "/Saves";
    paths_.mods = paths_.root + "/Mods";
    return FileSystem::exists(paths_.root);
}

bool ProjectLayout::create_default_structure() const {
    bool ok = true;
    for (const std::string& folder : standard_folders()) {
        ok = FileSystem::create_directories(folder) && ok;
    }
    return ok;
}

std::vector<std::string> ProjectLayout::standard_folders() const {
    return {paths_.assets,     paths_.characters, paths_.apartments, paths_.furniture,
            paths_.animations, paths_.dialogue,   paths_.events,     paths_.localization,
            paths_.saves,      paths_.mods};
}

} // namespace eve::content
