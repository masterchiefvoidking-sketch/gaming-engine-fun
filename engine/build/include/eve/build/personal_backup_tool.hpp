#pragma once

#include <eve/platform/backup_manager.hpp>

#include <string>
#include <vector>

namespace eve::build {

struct BackupToolResult {
    bool success = false;
    std::string path;
    std::string message;
};

class PersonalBackupTool {
public:
    BackupToolResult manual_backup(std::string_view project_root, std::string_view backup_root,
                                   std::string_view label) const;
    BackupToolResult auto_backup(std::string_view project_root, std::string_view backup_root,
                                 std::string_view label) const;
    BackupToolResult restore(std::string_view backup_path, std::string_view target_dir) const;
    [[nodiscard]] std::vector<platform::BackupEntry> list(std::string_view backup_root) const;

private:
    platform::BackupManager backup_manager_;
};

} // namespace eve::build
