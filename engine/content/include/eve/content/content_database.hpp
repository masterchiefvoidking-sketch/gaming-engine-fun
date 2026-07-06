#pragma once

#include <eve/content/content_types.hpp>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::content {

struct ContentSearchResult {
    std::string asset_id;
    AssetType type = AssetType::Character;
    std::string display_name;
    f32 relevance = 0.0f;
};

class ContentDatabase {
public:
    bool register_asset(ContentAsset asset);
    bool unregister_asset(std::string_view asset_id);
    [[nodiscard]] ContentAsset* find(std::string_view asset_id);
    [[nodiscard]] const ContentAsset* find(std::string_view asset_id) const;

    [[nodiscard]] std::vector<const ContentAsset*> assets_of_type(AssetType type) const;
    [[nodiscard]] std::vector<ContentSearchResult> search(std::string_view query) const;
    [[nodiscard]] std::size_t asset_count() const { return assets_.size(); }
    [[nodiscard]] std::size_t count_of_type(AssetType type) const;

    bool load_from_directory(std::string_view path, AssetType type);
    bool save_index(std::string_view path) const;
    bool load_index(std::string_view path);

private:
    std::unordered_map<std::string, ContentAsset> assets_;
};

} // namespace eve::content
