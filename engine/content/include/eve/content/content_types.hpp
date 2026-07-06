#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::content {

enum class AssetType : u8 {
    Character = 0,
    Apartment,
    Furniture,
    Animation,
    Material,
    Clothing,
    Hairstyle,
    Dialogue,
    Event,
    Story,
    Item,
    Audio,
    Lighting,
    CameraPreset,
    SavePreset,
    Localization,
    Expression,
    Pose,
    Mod,
    Plugin,
    Count
};

enum class ImportFormat : u8 {
    Fbx = 0,
    Gltf,
    Usd,
    Png,
    Jpg,
    Tga,
    Hdri,
    Wav,
    Ogg,
    Mp4,
    Json,
    Unknown,
};

struct ContentMetadata {
    std::string id;
    std::string display_name;
    std::string author;
    std::string version = "1.0.0";
    std::string description;
    std::vector<std::string> tags;
    std::string collection;
    std::string thumbnail_path;
    u64 created_at = 0;
    u64 modified_at = 0;
};

struct ContentAsset {
    AssetType type = AssetType::Character;
    ContentMetadata metadata;
    std::string source_path;
    std::string data_path;
    bool from_mod = false;
    std::string mod_id;
};

[[nodiscard]] const char* to_string(AssetType type);
[[nodiscard]] const char* to_string(ImportFormat format);
[[nodiscard]] ImportFormat detect_import_format(std::string_view path);

} // namespace eve::content
