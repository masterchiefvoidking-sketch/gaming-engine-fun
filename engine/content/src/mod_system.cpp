#include <algorithm>
#include <eve/content/mod_system.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::content {

bool ModSystem::load_mod(std::string_view mod_path, ContentDatabase& database) {
    const std::string manifest_path = std::string(mod_path) + "/mod.json";
    const std::string content = FileSystem::read_text_file(manifest_path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        ModManifest manifest;
        manifest.id = json.value("id", "");
        manifest.name = json.value("name", "");
        manifest.version = json.value("version", "1.0.0");
        manifest.author = json.value("author", "");
        manifest.description = json.value("description", "");
        if (json.contains("extends")) {
            for (const auto& ext : json["extends"]) {
                manifest.extends.push_back(ext.get<std::string>());
            }
        }
        mods_.push_back(manifest);

        const std::string assets_path = std::string(mod_path) + "/Assets";
        if (FileSystem::is_directory(assets_path + "/Characters")) {
            database.load_from_directory(assets_path + "/Characters", AssetType::Character);
        }
        if (FileSystem::is_directory(assets_path + "/Clothing")) {
            database.load_from_directory(assets_path + "/Clothing", AssetType::Clothing);
        }
        if (FileSystem::is_directory(assets_path + "/Dialogue")) {
            database.load_from_directory(assets_path + "/Dialogue", AssetType::Dialogue);
        }
        if (FileSystem::is_directory(assets_path + "/Events")) {
            database.load_from_directory(assets_path + "/Events", AssetType::Event);
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool ModSystem::unload_mod(std::string_view mod_id, ContentDatabase& database) {
    (void)database;
    const auto it = std::find_if(mods_.begin(), mods_.end(),
                                 [mod_id](const ModManifest& m) { return m.id == mod_id; });
    if (it == mods_.end()) {
        return false;
    }
    mods_.erase(it);
    return true;
}

bool ModSystem::is_mod_loaded(std::string_view mod_id) const {
    for (const ModManifest& mod : mods_) {
        if (mod.id == mod_id) {
            return true;
        }
    }
    return false;
}

} // namespace eve::content
