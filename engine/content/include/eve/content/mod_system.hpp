#pragma once

#include <eve/content/content_database.hpp>

#include <string>
#include <vector>

namespace eve::content {

struct ModManifest {
    std::string id;
    std::string name;
    std::string version = "1.0.0";
    std::string author;
    std::string description;
    std::vector<std::string> extends; // characters, outfits, apartments, stories, etc.
};

class ModSystem {
public:
    bool load_mod(std::string_view mod_path, ContentDatabase& database);
    bool unload_mod(std::string_view mod_id, ContentDatabase& database);
    [[nodiscard]] const std::vector<ModManifest>& loaded_mods() const { return mods_; }
    [[nodiscard]] bool is_mod_loaded(std::string_view mod_id) const;

private:
    std::vector<ModManifest> mods_;
};

} // namespace eve::content
