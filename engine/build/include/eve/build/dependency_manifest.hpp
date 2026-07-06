#pragma once

#include <eve/build/asset_cooker.hpp>

#include <string>
#include <vector>

namespace eve::build {

struct DependencyEntry {
    std::string id;
    std::string path;
    std::string asset_type;
    std::vector<std::string> dependencies;
};

struct DependencyManifest {
    std::string project_id;
    std::string profile;
    std::vector<DependencyEntry> entries;
};

class DependencyManifestWriter {
public:
    bool write(std::string_view output_path, const DependencyManifest& manifest) const;
    DependencyManifest build_from_cook(std::string_view project_id, std::string_view profile,
                                       const AssetCookResult& cook) const;
};

} // namespace eve::build
