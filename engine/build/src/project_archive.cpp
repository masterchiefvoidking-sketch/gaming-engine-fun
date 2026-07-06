#include <eve/build/project_archive.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>
#include <nlohmann/json.hpp>

namespace eve::build {

ArchiveResult ProjectArchive::export_archive(std::string_view project_root,
                                             std::string_view archive_path) const {
    ArchiveResult result;
    if (!FileSystem::is_directory(project_root)) {
        result.message = "Project root not found";
        return result;
    }

    const std::string dest = std::string(archive_path);
    FileSystem::create_directories(dest);

    std::size_t count = 0;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(project_root)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        const std::filesystem::path rel =
            std::filesystem::relative(entry.path(), project_root);
        const std::filesystem::path out = std::filesystem::path(dest) / rel;
        std::filesystem::create_directories(out.parent_path());
        std::filesystem::copy_file(entry.path(), out,
                                   std::filesystem::copy_options::overwrite_existing);
        ++count;
    }

    nlohmann::json manifest;
    manifest["format"] = "EVE_ARCHIVE";
    manifest["version"] = 1;
    manifest["project_root"] = std::string(project_root);
    manifest["file_count"] = count;
    manifest["offline_only"] = true;
    FileSystem::write_text_file(dest + "/archive_manifest.json", manifest.dump(2));

    result.success = count > 0;
    result.archive_path = dest;
    result.file_count = count;
    result.message = "Project archive exported";
    return result;
}

ArchiveResult ProjectArchive::import_archive(std::string_view archive_path,
                                             std::string_view dest_root) const {
    ArchiveResult result;
    const std::string archive(archive_path);
    if (!FileSystem::exists(archive + "/archive_manifest.json")) {
        result.message = "Not a valid EVE archive";
        return result;
    }

    FileSystem::create_directories(dest_root);
    std::size_t count = 0;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(archive)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        const std::filesystem::path rel =
            std::filesystem::relative(entry.path(), archive);
        if (rel.filename() == "archive_manifest.json") {
            continue;
        }
        const std::filesystem::path out = std::filesystem::path(std::string(dest_root)) / rel;
        std::filesystem::create_directories(out.parent_path());
        std::filesystem::copy_file(entry.path(), out,
                                   std::filesystem::copy_options::overwrite_existing);
        ++count;
    }

    result.success = count > 0;
    result.archive_path = std::string(dest_root);
    result.file_count = count;
    result.message = "Project archive imported";
    return result;
}

} // namespace eve::build
