#pragma once

#include <eve/build/project_archive.hpp>

#include <string>

namespace eve::product {

struct ArchiveOptions {
    bool include_saves = true;
    bool include_screenshots = true;
    bool exclude_build_output = true;
    bool exclude_backups = true;
};

struct ArchiveValidationResult {
    bool valid = false;
    std::string message;
    std::size_t file_count = 0;
};

class ProjectArchiveEnhanced {
public:
    build::ArchiveResult export_archive(std::string_view project_root,
                                        std::string_view archive_path,
                                        const ArchiveOptions& options) const;
    build::ArchiveResult import_archive(std::string_view archive_path,
                                        std::string_view dest_root) const;
    ArchiveValidationResult validate_archive(std::string_view archive_path) const;
    bool restore_archive(std::string_view archive_path, std::string_view dest_root) const;
};

} // namespace eve::product
