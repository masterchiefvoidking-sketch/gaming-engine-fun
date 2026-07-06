#include <eve/build/personal_content_pack.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>
#include <nlohmann/json.hpp>

namespace eve::build {

ContentPackResult PersonalContentPack::export_pack(std::string_view source_dir,
                                                   std::string_view output_path,
                                                   const ContentPackManifest& manifest) const {
    ContentPackResult result;
    if (!FileSystem::is_directory(source_dir)) {
        result.message = "Pack source not found";
        return result;
    }

    const std::string out = std::string(output_path);
    FileSystem::create_directories(out);
    for (const std::string& include : manifest.includes) {
        const std::filesystem::path src = std::filesystem::path(std::string(source_dir)) / include;
        if (!std::filesystem::exists(src)) {
            continue;
        }
        if (std::filesystem::is_directory(src)) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(src)) {
                if (!entry.is_regular_file()) {
                    continue;
                }
                const std::filesystem::path rel =
                    std::filesystem::relative(entry.path(), source_dir);
                const std::filesystem::path dest = std::filesystem::path(out) / rel;
                std::filesystem::create_directories(dest.parent_path());
                std::filesystem::copy_file(entry.path(), dest,
                                           std::filesystem::copy_options::overwrite_existing);
            }
        } else {
            const std::filesystem::path dest = std::filesystem::path(out) / include;
            std::filesystem::create_directories(dest.parent_path());
            std::filesystem::copy_file(src, dest,
                                       std::filesystem::copy_options::overwrite_existing);
        }
    }

    nlohmann::json json;
    json["id"] = manifest.id;
    json["name"] = manifest.name;
    json["version"] = manifest.version;
    json["includes"] = manifest.includes;
    json["local_only"] = true;
    FileSystem::write_text_file(out + "/pack_manifest.json", json.dump(2));

    result.success = true;
    result.pack_path = out;
    result.message = "Content pack exported";
    return result;
}

ContentPackResult PersonalContentPack::import_pack(std::string_view pack_path,
                                                   std::string_view dest_project) const {
    ContentPackResult result;
    const std::string pack(pack_path);
    if (!FileSystem::exists(pack + "/pack_manifest.json")) {
        result.message = "Invalid content pack";
        return result;
    }

    FileSystem::create_directories(dest_project);
    for (const auto& entry : std::filesystem::recursive_directory_iterator(pack)) {
        if (!entry.is_regular_file() || entry.path().filename() == "pack_manifest.json") {
            continue;
        }
        const std::filesystem::path rel = std::filesystem::relative(entry.path(), pack);
        const std::filesystem::path out =
            std::filesystem::path(std::string(dest_project)) / "Assets" / rel;
        std::filesystem::create_directories(out.parent_path());
        std::filesystem::copy_file(entry.path(), out,
                                   std::filesystem::copy_options::overwrite_existing);
    }

    result.success = true;
    result.pack_path = std::string(dest_project);
    result.message = "Content pack imported";
    return result;
}

} // namespace eve::build
