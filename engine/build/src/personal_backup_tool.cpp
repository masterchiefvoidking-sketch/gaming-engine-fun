#include <eve/build/personal_backup_tool.hpp>

namespace eve::build {

BackupToolResult PersonalBackupTool::manual_backup(std::string_view project_root,
                                                   std::string_view backup_root,
                                                   std::string_view label) const {
    BackupToolResult result;
    if (!backup_manager_.create_backup(project_root, backup_root, label)) {
        result.message = "Manual backup failed";
        return result;
    }
    result.success = true;
    result.path = std::string(backup_root);
    result.message = "Manual backup created";
    return result;
}

BackupToolResult PersonalBackupTool::auto_backup(std::string_view project_root,
                                                 std::string_view backup_root,
                                                 std::string_view label) const {
    return manual_backup(project_root, backup_root, std::string(label) + "_auto");
}

BackupToolResult PersonalBackupTool::restore(std::string_view backup_path,
                                             std::string_view target_dir) const {
    BackupToolResult result;
    if (!backup_manager_.restore_backup(backup_path, target_dir)) {
        result.message = "Restore failed";
        return result;
    }
    result.success = true;
    result.path = std::string(target_dir);
    result.message = "Backup restored";
    return result;
}

std::vector<platform::BackupEntry> PersonalBackupTool::list(std::string_view backup_root) const {
    return backup_manager_.list_backups(backup_root);
}

} // namespace eve::build
