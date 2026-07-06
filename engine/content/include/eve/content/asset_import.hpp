#pragma once

#include <eve/content/content_types.hpp>

#include <string>

namespace eve::content {

struct ImportResult {
    bool success = false;
    std::string asset_id;
    ImportFormat format = ImportFormat::Unknown;
    std::string output_path;
    std::string message;
};

class AssetImporter {
public:
    ImportResult import_file(std::string_view source_path, std::string_view output_dir,
                             AssetType target_type) const;
    [[nodiscard]] bool supports_format(ImportFormat format) const;
};

} // namespace eve::content
