#include <eve/build/dependency_manifest.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::build {

DependencyManifest DependencyManifestWriter::build_from_cook(std::string_view project_id,
                                                             std::string_view profile,
                                                             const AssetCookResult& cook) const {
    DependencyManifest manifest;
    manifest.project_id = std::string(project_id);
    manifest.profile = std::string(profile);
    for (const CookedAssetEntry& asset : cook.assets) {
        DependencyEntry entry;
        entry.id = asset.cooked_path;
        entry.path = asset.cooked_path;
        entry.asset_type = asset.asset_type;
        if (asset.asset_type == "dialogue" || asset.asset_type == "event") {
            entry.dependencies.push_back("character");
        }
        manifest.entries.push_back(std::move(entry));
    }
    return manifest;
}

bool DependencyManifestWriter::write(std::string_view output_path,
                                     const DependencyManifest& manifest) const {
    nlohmann::json json;
    json["project_id"] = manifest.project_id;
    json["profile"] = manifest.profile;
    json["dependencies"] = nlohmann::json::array();
    for (const DependencyEntry& entry : manifest.entries) {
        json["dependencies"].push_back({{"id", entry.id},
                                        {"path", entry.path},
                                        {"type", entry.asset_type},
                                        {"deps", entry.dependencies}});
    }
    return FileSystem::write_text_file(output_path, json.dump(2));
}

} // namespace eve::build
