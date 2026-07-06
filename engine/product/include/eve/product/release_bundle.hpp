#pragma once

#include <string>
#include <vector>

namespace eve::product {

struct ReleaseBundleResult {
    bool success = false;
    std::string bundle_root;
    std::vector<std::string> artifacts;
    std::string message;
};

struct ReleaseBundleConfig {
    std::string game_root;
    std::string data_root;
    std::string output_root;
    std::string bundle_name = "ApartmentLifeDemo_RC1";
};

class ReleaseBundle {
public:
    ReleaseBundleResult create(const ReleaseBundleConfig& config) const;
};

} // namespace eve::product
