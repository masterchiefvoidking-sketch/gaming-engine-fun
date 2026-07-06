#pragma once

#include <string>
#include <vector>

namespace eve::build {

struct ContentPackManifest {
    std::string id;
    std::string name;
    std::string version = "1.0.0";
    std::vector<std::string> includes;
};

struct ContentPackResult {
    bool success = false;
    std::string pack_path;
    std::string message;
};

class PersonalContentPack {
public:
    ContentPackResult export_pack(std::string_view source_dir, std::string_view output_path,
                                  const ContentPackManifest& manifest) const;
    ContentPackResult import_pack(std::string_view pack_path,
                                  std::string_view dest_project) const;
};

} // namespace eve::build
