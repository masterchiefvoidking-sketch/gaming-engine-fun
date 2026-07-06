#pragma once

#include <eve/build/export_profiles.hpp>

#include <string>
#include <vector>

namespace eve::build {

struct CookedAssetEntry {
    std::string source_path;
    std::string cooked_path;
    std::string asset_type;
    std::size_t size_bytes = 0;
    bool compressed = false;
};

struct AssetCookResult {
    bool success = false;
    std::string output_dir;
    std::vector<CookedAssetEntry> assets;
    std::string message;
};

class AssetCooker {
public:
    AssetCookResult cook(std::string_view project_root, std::string_view output_dir,
                         const ExportProfileSettings& profile) const;
};

} // namespace eve::build
