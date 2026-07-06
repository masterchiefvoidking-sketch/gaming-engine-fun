#include <eve/content/asset_import.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::content {

ImportResult AssetImporter::import_file(std::string_view source_path, std::string_view output_dir,
                                        AssetType target_type) const {
    ImportResult result;
    result.format = detect_import_format(source_path);
    if (!supports_format(result.format)) {
        result.message = "Unsupported format";
        return result;
    }

    const std::string filename = std::string(source_path).substr(source_path.rfind('/') + 1);
    const std::string dest = std::string(output_dir) + "/" + filename;

    const std::string content = FileSystem::read_text_file(source_path);
    if (content.empty() && result.format != ImportFormat::Fbx &&
        result.format != ImportFormat::Gltf && result.format != ImportFormat::Usd) {
        result.message = "Failed to read source file";
        return result;
    }

    FileSystem::create_directories(output_dir);
    if (result.format == ImportFormat::Json) {
        if (!FileSystem::write_text_file(dest, content)) {
            result.message = "Failed to write output";
            return result;
        }
    } else {
        nlohmann::json meta = {{"id", filename},
                               {"source", std::string(source_path)},
                               {"type", to_string(target_type)},
                               {"format", to_string(result.format)},
                               {"imported", true}};
        const std::string meta_path = std::string(output_dir) + "/" +
                                      filename.substr(0, filename.rfind('.')) + ".meta.json";
        FileSystem::write_text_file(meta_path, meta.dump(2));
        result.output_path = meta_path;
    }

    result.success = true;
    result.asset_id = filename.substr(0, filename.rfind('.'));
    result.output_path = dest;
    result.message = "Imported successfully";
    return result;
}

bool AssetImporter::supports_format(ImportFormat format) const {
    return format != ImportFormat::Unknown;
}

} // namespace eve::content
