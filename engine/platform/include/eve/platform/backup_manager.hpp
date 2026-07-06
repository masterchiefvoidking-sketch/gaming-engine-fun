#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::platform {

struct BackupEntry {
    std::string path;
    std::string label;
    u64 timestamp = 0;
    std::size_t size_bytes = 0;
};

class BackupManager {
public:
    bool create_backup(std::string_view source_dir, std::string_view backup_root,
                       std::string_view label) const;
    [[nodiscard]] std::vector<BackupEntry> list_backups(std::string_view backup_root) const;
    bool restore_backup(std::string_view backup_path, std::string_view target_dir) const;
};

} // namespace eve::platform
