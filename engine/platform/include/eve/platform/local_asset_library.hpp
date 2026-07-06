#pragma once

#include <eve/content/content_database.hpp>

#include <string>

namespace eve::platform {

// Local asset library — NOT a marketplace. Offline-first reusable packs.
class LocalAssetLibrary {
public:
    bool scan(std::string_view library_root);
    [[nodiscard]] const content::ContentDatabase& database() const { return database_; }
    bool import_pack(std::string_view pack_path, std::string_view dest_project) const;
    bool export_pack(std::string_view asset_ids, std::string_view output_path) const;

private:
    content::ContentDatabase database_;
    std::string library_root_;
};

} // namespace eve::platform
