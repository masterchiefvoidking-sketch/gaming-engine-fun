#include <eve/product/project_archive_enhanced.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>
#include <nlohmann/json.hpp>

namespace eve::product {

namespace {

bool should_skip(const std::filesystem::path& rel, const ArchiveOptions& options) {
    const std::string path = rel.generic_string();
    if (options.exclude_build_output && path.find("Build/") == 0) {
        return true;
    }
    if (options.exclude_backups && path.find("Backups/") == 0) {
        return true;
    }
    if (!options.include_saves && path.find("Saves/") == 0) {
        return true;
    }
    if (!options.include_screenshots && path.find("Screenshots/") == 0) {
        return true;
    }
    return false;
}

} // namespace

build::ArchiveResult ProjectArchiveEnhanced::export_archive(std::string_view project_root,
                                                            std::string_view archive_path,
                                                            const ArchiveOptions& options) const {
    build::ArchiveResult result;
    if (!FileSystem::is_directory(project_root)) {
        result.message = "Project root not found";
        return result;
    }

    const std::string dest = std::string(archive_path);
    (void)FileSystem::create_directories(dest);

    std::size_t count = 0;
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(std::string(project_root))) {
        if (!entry.is_regular_file()) {
            continue;
        }
        const std::filesystem::path rel =
            std::filesystem::relative(entry.path(), std::string(project_root));
        if (should_skip(rel, options)) {
            continue;
        }
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
    manifest["include_saves"] = options.include_saves;
    manifest["include_screenshots"] = options.include_screenshots;
    manifest["offline_only"] = true;
    (void)FileSystem::write_text_file(dest + "/archive_manifest.json", manifest.dump(2));

    result.success = count > 0;
    result.archive_path = dest;
    result.file_count = count;
    result.message = "Enhanced project archive exported";
    return result;
}

build::ArchiveResult ProjectArchiveEnhanced::import_archive(std::string_view archive_path,
                                                            std::string_view dest_root) const {
    build::ProjectArchive base;
    return base.import_archive(archive_path, dest_root);
}

ArchiveValidationResult ProjectArchiveEnhanced::validate_archive(
    std::string_view archive_path) const {
    ArchiveValidationResult result;
    const std::string archive(archive_path);
    const std::string manifest_path = archive + "/archive_manifest.json";
    if (!FileSystem::exists(manifest_path)) {
        result.message = "Missing archive manifest";
        return result;
    }
    try {
        const auto json = nlohmann::json::parse(FileSystem::read_text_file(manifest_path));
        if (json.value("format", "") != "EVE_ARCHIVE") {
            result.message = "Invalid archive format";
            return result;
        }
        result.file_count = json.value("file_count", 0);
        result.valid = result.file_count > 0;
        result.message = result.valid ? "Archive valid" : "Archive empty";
        return result;
    } catch (const nlohmann::json::exception&) {
        result.message = "Invalid manifest JSON";
        return result;
    }
}

bool ProjectArchiveEnhanced::restore_archive(std::string_view archive_path,
                                             std::string_view dest_root) const {
    const ArchiveValidationResult validation = validate_archive(archive_path);
    if (!validation.valid) {
        return false;
    }
    return import_archive(archive_path, dest_root).success;
}

} // namespace eve::product
