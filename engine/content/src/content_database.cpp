#include <algorithm>
#include <cctype>
#include <eve/content/content_database.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::content {

namespace {

std::string to_lower(std::string value) {
    for (char& c : value) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return value;
}

} // namespace

bool ContentDatabase::register_asset(ContentAsset asset) {
    if (asset.metadata.id.empty()) {
        return false;
    }
    assets_[asset.metadata.id] = std::move(asset);
    return true;
}

bool ContentDatabase::unregister_asset(std::string_view asset_id) {
    return assets_.erase(std::string(asset_id)) > 0;
}

ContentAsset* ContentDatabase::find(std::string_view asset_id) {
    const auto it = assets_.find(std::string(asset_id));
    if (it == assets_.end()) {
        return nullptr;
    }
    return &it->second;
}

const ContentAsset* ContentDatabase::find(std::string_view asset_id) const {
    const auto it = assets_.find(std::string(asset_id));
    if (it == assets_.end()) {
        return nullptr;
    }
    return &it->second;
}

std::vector<const ContentAsset*> ContentDatabase::assets_of_type(AssetType type) const {
    std::vector<const ContentAsset*> result;
    for (const auto& [_, asset] : assets_) {
        if (asset.type == type) {
            result.push_back(&asset);
        }
    }
    return result;
}

std::vector<ContentSearchResult> ContentDatabase::search(std::string_view query) const {
    std::vector<ContentSearchResult> results;
    const std::string lower_query = to_lower(std::string(query));
    if (lower_query.empty()) {
        return results;
    }
    for (const auto& [id, asset] : assets_) {
        (void)id;
        f32 score = 0.0f;
        const std::string name = to_lower(asset.metadata.display_name);
        const std::string desc = to_lower(asset.metadata.description);
        if (name.find(lower_query) != std::string::npos) {
            score += 1.0f;
        }
        if (desc.find(lower_query) != std::string::npos) {
            score += 0.5f;
        }
        for (const std::string& tag : asset.metadata.tags) {
            if (to_lower(tag).find(lower_query) != std::string::npos) {
                score += 0.3f;
            }
        }
        if (score > 0.0f) {
            results.push_back({asset.metadata.id, asset.type, asset.metadata.display_name, score});
        }
    }
    std::sort(results.begin(), results.end(),
              [](const ContentSearchResult& a, const ContentSearchResult& b) {
                  return a.relevance > b.relevance;
              });
    return results;
}

std::size_t ContentDatabase::count_of_type(AssetType type) const {
    return assets_of_type(type).size();
}

bool ContentDatabase::load_from_directory(std::string_view path, AssetType type) {
    const std::vector<std::string> files = FileSystem::list_directory(path);
    for (const std::string& file : files) {
        if (file.size() < 6 || file.substr(file.size() - 5) != ".json") {
            continue;
        }
        const std::string full_path = std::string(path) + "/" + file;
        const std::string content = FileSystem::read_text_file(full_path);
        if (content.empty()) {
            continue;
        }
        try {
            const auto json = nlohmann::json::parse(content);
            ContentAsset asset;
            asset.type = type;
            asset.metadata.id = json.value("id", file.substr(0, file.size() - 5));
            asset.metadata.display_name = json.value("name", asset.metadata.id);
            asset.metadata.description = json.value("description", "");
            asset.metadata.author = json.value("author", "");
            asset.metadata.collection = json.value("collection", "");
            asset.metadata.thumbnail_path = json.value("thumbnail", "");
            if (json.contains("tags")) {
                for (const auto& tag : json["tags"]) {
                    asset.metadata.tags.push_back(tag.get<std::string>());
                }
            }
            asset.data_path = full_path;
            register_asset(std::move(asset));
        } catch (const nlohmann::json::exception&) {
        }
    }
    return true;
}

bool ContentDatabase::save_index(std::string_view path) const {
    nlohmann::json json = nlohmann::json::array();
    for (const auto& [_, asset] : assets_) {
        json.push_back({{"id", asset.metadata.id},
                        {"name", asset.metadata.display_name},
                        {"type", to_string(asset.type)},
                        {"path", asset.data_path},
                        {"mod", asset.mod_id}});
    }
    return FileSystem::write_text_file(path, json.dump(2));
}

bool ContentDatabase::load_index(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            ContentAsset asset;
            asset.metadata.id = entry.value("id", "");
            asset.metadata.display_name = entry.value("name", "");
            asset.data_path = entry.value("path", "");
            asset.mod_id = entry.value("mod", "");
            asset.from_mod = !asset.mod_id.empty();
            const std::string type_str = entry.value("type", "character");
            if (type_str == "furniture") asset.type = AssetType::Furniture;
            else if (type_str == "clothing") asset.type = AssetType::Clothing;
            else if (type_str == "dialogue") asset.type = AssetType::Dialogue;
            else if (type_str == "event") asset.type = AssetType::Event;
            register_asset(std::move(asset));
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

} // namespace eve::content
