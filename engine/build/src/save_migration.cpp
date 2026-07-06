#include <eve/build/save_migration.hpp>

#include <eve/core/filesystem/filesystem.hpp>
#include <eve/platform/unified_save.hpp>

#include <nlohmann/json.hpp>

namespace eve::build {

bool SaveMigration::needs_migration(std::string_view save_path) const {
    const std::string content = FileSystem::read_text_file(save_path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        if (json.value("magic", "") != platform::kUnifiedSaveMagic) {
            return true;
        }
        return json.value("version", 0u) < platform::kUnifiedSaveVersion;
    } catch (const nlohmann::json::exception&) {
        return true;
    }
}

SaveMigrationResult SaveMigration::migrate(std::string_view save_path) const {
    SaveMigrationResult result;
    const std::string content = FileSystem::read_text_file(save_path);
    if (content.empty()) {
        result.message = "Save file empty";
        return result;
    }

    try {
        auto json = nlohmann::json::parse(content);
        result.from_version = json.value("version", 0u);

        if (json.value("magic", "") != platform::kUnifiedSaveMagic) {
            json["magic"] = platform::kUnifiedSaveMagic;
            json["version"] = platform::kUnifiedSaveVersion;
            if (!json.contains("settings")) {
                json["settings"] = {{"locale", "en"}, {"quality", 2}};
            }
        } else if (result.from_version < platform::kUnifiedSaveVersion) {
            json["version"] = platform::kUnifiedSaveVersion;
            if (!json.contains("wardrobe")) {
                json["wardrobe"] = {{"active", ""}, {"owned", nlohmann::json::array()}};
            }
            if (!json.contains("photos")) {
                json["photos"] = nlohmann::json::array();
            }
        } else {
            result.success = true;
            result.message = "Already current";
            return result;
        }

        if (!FileSystem::write_text_file(save_path, json.dump(2))) {
            result.message = "Failed to write migrated save";
            return result;
        }

        result.success = true;
        result.to_version = platform::kUnifiedSaveVersion;
        result.message = "Save migrated";
        return result;
    } catch (const nlohmann::json::exception&) {
        result.message = "Invalid save JSON";
        return result;
    }
}

SaveMigrationResult SaveMigration::migrate_directory(std::string_view saves_dir) const {
    SaveMigrationResult result;
    if (!FileSystem::is_directory(saves_dir)) {
        result.message = "Saves directory not found";
        return result;
    }

    u32 migrated = 0;
    for (const std::string& file : FileSystem::list_directory(saves_dir)) {
        if (file.size() < 6 || file.substr(file.size() - 5) != ".json") {
            continue;
        }
        const std::string path = std::string(saves_dir) + "/" + file;
        if (needs_migration(path)) {
            const SaveMigrationResult single = migrate(path);
            if (single.success) {
                ++migrated;
            }
        }
    }

    result.success = true;
    result.message = "Migrated " + std::to_string(migrated) + " save(s)";
    return result;
}

} // namespace eve::build
