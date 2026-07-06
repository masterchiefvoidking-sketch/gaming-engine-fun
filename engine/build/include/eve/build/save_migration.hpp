#pragma once

#include <eve/platform/unified_save.hpp>

#include <string>

namespace eve::build {

struct SaveMigrationResult {
    bool success = false;
    u32 from_version = 0;
    u32 to_version = platform::kUnifiedSaveVersion;
    std::string message;
};

class SaveMigration {
public:
    [[nodiscard]] bool needs_migration(std::string_view save_path) const;
    SaveMigrationResult migrate(std::string_view save_path) const;
    SaveMigrationResult migrate_directory(std::string_view saves_dir) const;
};

} // namespace eve::build
