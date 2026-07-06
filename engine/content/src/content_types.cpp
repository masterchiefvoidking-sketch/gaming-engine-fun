#include <algorithm>
#include <eve/content/content_types.hpp>

namespace eve::content {

const char* to_string(AssetType type) {
    switch (type) {
    case AssetType::Apartment:
        return "apartment";
    case AssetType::Furniture:
        return "furniture";
    case AssetType::Animation:
        return "animation";
    case AssetType::Material:
        return "material";
    case AssetType::Clothing:
        return "clothing";
    case AssetType::Hairstyle:
        return "hairstyle";
    case AssetType::Dialogue:
        return "dialogue";
    case AssetType::Event:
        return "event";
    case AssetType::Story:
        return "story";
    case AssetType::Item:
        return "item";
    case AssetType::Audio:
        return "audio";
    case AssetType::Lighting:
        return "lighting";
    case AssetType::CameraPreset:
        return "camera_preset";
    case AssetType::SavePreset:
        return "save_preset";
    case AssetType::Localization:
        return "localization";
    case AssetType::Expression:
        return "expression";
    case AssetType::Pose:
        return "pose";
    case AssetType::Mod:
        return "mod";
    case AssetType::Plugin:
        return "plugin";
    default:
        return "character";
    }
}

const char* to_string(ImportFormat format) {
    switch (format) {
    case ImportFormat::Fbx:
        return "fbx";
    case ImportFormat::Gltf:
        return "gltf";
    case ImportFormat::Usd:
        return "usd";
    case ImportFormat::Png:
        return "png";
    case ImportFormat::Jpg:
        return "jpg";
    case ImportFormat::Tga:
        return "tga";
    case ImportFormat::Hdri:
        return "hdri";
    case ImportFormat::Wav:
        return "wav";
    case ImportFormat::Ogg:
        return "ogg";
    case ImportFormat::Mp4:
        return "mp4";
    case ImportFormat::Json:
        return "json";
    default:
        return "unknown";
    }
}

ImportFormat detect_import_format(std::string_view path) {
    const auto dot = path.rfind('.');
    if (dot == std::string_view::npos) {
        return ImportFormat::Unknown;
    }
    const std::string ext(path.substr(dot + 1));
    if (ext == "fbx") return ImportFormat::Fbx;
    if (ext == "gltf" || ext == "glb") return ImportFormat::Gltf;
    if (ext == "usd" || ext == "usda" || ext == "usdc") return ImportFormat::Usd;
    if (ext == "png") return ImportFormat::Png;
    if (ext == "jpg" || ext == "jpeg") return ImportFormat::Jpg;
    if (ext == "tga") return ImportFormat::Tga;
    if (ext == "hdr" || ext == "hdri") return ImportFormat::Hdri;
    if (ext == "wav") return ImportFormat::Wav;
    if (ext == "ogg") return ImportFormat::Ogg;
    if (ext == "mp4") return ImportFormat::Mp4;
    if (ext == "json") return ImportFormat::Json;
    return ImportFormat::Unknown;
}

} // namespace eve::content
