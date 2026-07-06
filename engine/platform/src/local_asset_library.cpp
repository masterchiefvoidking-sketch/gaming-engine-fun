#include <eve/platform/local_asset_library.hpp>
#include <eve/core/filesystem/filesystem.hpp>

namespace eve::platform {

bool LocalAssetLibrary::scan(std::string_view library_root) {
    library_root_ = std::string(library_root);
    if (!FileSystem::is_directory(library_root)) {
        return false;
    }
    database_.load_from_directory(library_root_ + "/Characters", content::AssetType::Character);
    database_.load_from_directory(library_root_ + "/Clothing", content::AssetType::Clothing);
    database_.load_from_directory(library_root_ + "/Furniture", content::AssetType::Furniture);
    database_.load_from_directory(library_root_ + "/Dialogue", content::AssetType::Dialogue);
    return database_.asset_count() > 0;
}

bool LocalAssetLibrary::import_pack(std::string_view pack_path,
                                    std::string_view dest_project) const {
    if (!FileSystem::is_directory(pack_path)) {
        return false;
    }
    (void)dest_project;
    return true;
}

bool LocalAssetLibrary::export_pack(std::string_view asset_ids, std::string_view output_path) const {
    (void)asset_ids;
    return FileSystem::create_directories(output_path);
}

} // namespace eve::platform
