#include <eve/platform/backup_manager.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <chrono>
#include <filesystem>

namespace eve::platform {

namespace {

bool should_skip_backup_path(const std::filesystem::path& rel) {
    for (const std::filesystem::path& part : rel) {
        const std::string name = part.string();
        if (name == "Backups" || name == "Build" || name == ".git") {
            return true;
        }
    }
    return false;
}

} // namespace

bool BackupManager::create_backup(std::string_view source_dir, std::string_view backup_root,
                                  std::string_view label) const {
    if (!FileSystem::is_directory(source_dir)) {
        return false;
    }
    const auto now = std::chrono::system_clock::now().time_since_epoch().count();
    const std::string dest =
        std::string(backup_root) + "/" + std::string(label) + "_" + std::to_string(now);
    FileSystem::create_directories(dest);

    for (const auto& entry : std::filesystem::recursive_directory_iterator(source_dir)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        const std::filesystem::path rel = std::filesystem::relative(entry.path(), source_dir);
        if (should_skip_backup_path(rel)) {
            continue;
        }
        const std::filesystem::path out = std::filesystem::path(dest) / rel;
        std::filesystem::create_directories(out.parent_path());
        std::filesystem::copy_file(entry.path(), out, std::filesystem::copy_options::overwrite_existing);
    }
    return true;
}

std::vector<BackupEntry> BackupManager::list_backups(std::string_view backup_root) const {
    std::vector<BackupEntry> entries;
    if (!FileSystem::is_directory(backup_root)) {
        return entries;
    }
    for (const std::string& name : FileSystem::list_directory(backup_root)) {
        BackupEntry entry;
        entry.path = std::string(backup_root) + "/" + name;
        entry.label = name;
        entries.push_back(std::move(entry));
    }
    return entries;
}

bool BackupManager::restore_backup(std::string_view backup_path,
                                   std::string_view target_dir) const {
    if (!FileSystem::is_directory(backup_path)) {
        return false;
    }
    FileSystem::create_directories(target_dir);
    for (const auto& entry : std::filesystem::recursive_directory_iterator(backup_path)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        const std::filesystem::path rel = std::filesystem::relative(entry.path(), backup_path);
        const std::filesystem::path out = std::filesystem::path(target_dir) / rel;
        std::filesystem::create_directories(out.parent_path());
        std::filesystem::copy_file(entry.path(), out, std::filesystem::copy_options::overwrite_existing);
    }
    return true;
}

} // namespace eve::platform
