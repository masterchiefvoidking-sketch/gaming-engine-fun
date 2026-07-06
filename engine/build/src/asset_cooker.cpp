#include <eve/build/asset_cooker.hpp>

#include <eve/core/filesystem/filesystem.hpp>

#include <filesystem>
#include <nlohmann/json.hpp>

namespace eve::build {

namespace {

std::string detect_asset_type(const std::filesystem::path& path) {
    const std::string ext = path.extension().string();
    if (ext == ".json") {
        const std::string parent = path.parent_path().filename().string();
        if (parent == "Dialogue") {
            return "dialogue";
        }
        if (parent == "Events") {
            return "event";
        }
        if (parent == "Wardrobe" || parent == "Clothing") {
            return "clothing";
        }
        if (parent == "Characters") {
            return "character";
        }
        if (parent == "Furniture") {
            return "furniture";
        }
        if (parent == "Apartments") {
            return "apartment";
        }
        return "data";
    }
    if (ext == ".png" || ext == ".jpg" || ext == ".webp") {
        return "texture";
    }
    if (ext == ".wav" || ext == ".ogg" || ext == ".mp3") {
        return "audio";
    }
    if (ext == ".glb" || ext == ".gltf" || ext == ".fbx") {
        return "mesh";
    }
    return "unknown";
}

} // namespace

AssetCookResult AssetCooker::cook(std::string_view project_root, std::string_view output_dir,
                                  const ExportProfileSettings& profile) const {
    AssetCookResult result;
    const std::filesystem::path assets_root =
        std::filesystem::path(std::string(project_root)) / "Assets";
    if (!FileSystem::is_directory(assets_root.string())) {
        result.message = "Assets folder not found";
        return result;
    }

    const std::string cooked_root = std::string(output_dir) + "/cooked";
    FileSystem::create_directories(cooked_root);

    for (const auto& entry : std::filesystem::recursive_directory_iterator(assets_root)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        const std::filesystem::path rel =
            std::filesystem::relative(entry.path(), assets_root);
        const std::filesystem::path out = std::filesystem::path(cooked_root) / rel;
        std::filesystem::create_directories(out.parent_path());
        std::filesystem::copy_file(entry.path(), out,
                                   std::filesystem::copy_options::overwrite_existing);

        CookedAssetEntry cooked;
        cooked.source_path = entry.path().string();
        cooked.cooked_path = out.string();
        cooked.asset_type = detect_asset_type(entry.path());
        cooked.size_bytes = entry.file_size();
        cooked.compressed = profile.compress_assets &&
                            (cooked.asset_type == "texture" || cooked.asset_type == "audio");
        result.assets.push_back(std::move(cooked));
    }

    nlohmann::json runtime;
    runtime["max_texture_size"] = profile.max_texture_size;
    runtime["shader_quality"] = profile.shader_quality;
    runtime["hair_quality"] = profile.hair_quality;
    runtime["cloth_quality"] = profile.cloth_quality;
    runtime["compress_assets"] = profile.compress_assets;
    runtime["asset_count"] = result.assets.size();
    FileSystem::write_text_file(cooked_root + "/runtime_package.json", runtime.dump(2));

    nlohmann::json shader_variants;
    shader_variants["quality_tier"] = profile.shader_quality;
    shader_variants["renderer"] = profile.renderer;
    shader_variants["variants"] = {"skin", "hair", "cloth", "toon_outline"};
    FileSystem::write_text_file(cooked_root + "/shader_variants.json", shader_variants.dump(2));

    nlohmann::json animation_clips = nlohmann::json::array();
    for (const CookedAssetEntry& asset : result.assets) {
        if (asset.asset_type == "data" && asset.source_path.find("Animations") != std::string::npos) {
            animation_clips.push_back({{"path", asset.cooked_path}, {"compressed", asset.compressed}});
        }
    }
    FileSystem::write_text_file(cooked_root + "/animation_clips.json", animation_clips.dump(2));

    result.success = !result.assets.empty();
    result.output_dir = cooked_root;
    result.message = result.success ? "Assets cooked" : "No assets found";
    return result;
}

} // namespace eve::build
