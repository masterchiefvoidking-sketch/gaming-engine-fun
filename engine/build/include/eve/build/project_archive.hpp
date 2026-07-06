#pragma once

#include <string>

namespace eve::build {

struct ArchiveResult {
    bool success = false;
    std::string archive_path;
    std::size_t file_count = 0;
    std::string message;
};

class ProjectArchive {
public:
    ArchiveResult export_archive(std::string_view project_root, std::string_view archive_path) const;
    ArchiveResult import_archive(std::string_view archive_path, std::string_view dest_root) const;
};

} // namespace eve::build
