#include <eve/content/game_content_project.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/core/logging/logger.hpp>

#include <nlohmann/json.hpp>

namespace eve::content {

bool GameContentProject::open(std::string_view game_root) {
    if (!layout_.initialize(game_root)) {
        return false;
    }
    const std::string manifest = layout_.paths().root + "/project.json";
    const std::string manifest_content = FileSystem::read_text_file(manifest);
    if (!manifest_content.empty()) {
        try {
            const auto json = nlohmann::json::parse(manifest_content);
            layout_.set_title(json.value("title", "Untitled Project"));
        } catch (const nlohmann::json::exception&) {
        }
    }
    return load_all();
}

bool GameContentProject::create_new(std::string_view game_root, std::string_view title) {
    layout_.initialize(game_root);
    layout_.set_title(title);
    if (!layout_.create_default_structure()) {
        return false;
    }
    EVE_LOG(Info, "Content", "Created project: ", title);
    return true;
}

bool GameContentProject::load_all() {
    const ProjectPaths& paths = layout_.paths();

    auto load_dir = [&](std::string_view dir, AssetType type) {
        if (FileSystem::is_directory(dir)) {
            database_.load_from_directory(dir, type);
        }
    };
    load_dir(paths.characters, AssetType::Character);
    load_dir(paths.furniture, AssetType::Furniture);
    load_dir(paths.dialogue, AssetType::Dialogue);
    load_dir(paths.events, AssetType::Event);

    auto load_json_dir = [&](const std::string& dir, const auto& loader) {
        if (!FileSystem::is_directory(dir)) {
            return;
        }
        for (const std::string& file : FileSystem::list_directory(dir)) {
            if (file.size() > 5 && file.substr(file.size() - 5) == ".json") {
                loader(dir + "/" + file);
            }
        }
    };

    load_json_dir(paths.characters, [this](const std::string& p) { characters_.load_character(p); });
    load_json_dir(paths.characters + "/Wardrobe",
                  [this](const std::string& p) { wardrobe_.load_item(p); });
    load_json_dir(paths.furniture, [this](const std::string& p) { apartments_.load_furniture(p); });
    load_json_dir(paths.apartments, [this](const std::string& p) { apartments_.load_apartment(p); });
    load_json_dir(paths.dialogue, [this](const std::string& p) { dialogue_.load_package(p); });
    load_json_dir(paths.events, [this](const std::string& p) { events_.load_event(p); });

    const std::string loc_path = paths.localization + "/strings.json";
    if (FileSystem::exists(loc_path)) {
        localization_.load(loc_path);
    }

    if (FileSystem::is_directory(paths.dialogue)) {
        hot_reload_.watch(paths.dialogue);
    }
    if (FileSystem::is_directory(paths.events)) {
        hot_reload_.watch(paths.events);
    }
    return true;
}

bool GameContentProject::save_all() const {
    const ProjectPaths& paths = layout_.paths();
    database_.save_index(paths.assets + "/content_index.json");
    localization_.save(paths.localization + "/strings.json");
    return true;
}

bool GameContentProject::export_build(const PackagingConfig& config) const {
    const PackagingResult result = packager_.export_project(config, layout_.paths().root);
    return result.success;
}

} // namespace eve::content
