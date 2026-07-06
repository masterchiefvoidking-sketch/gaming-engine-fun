#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::content {

enum class ExportTarget : u8 {
    Windows = 0,
    Linux,
    SteamDeck,
};

struct PackagingConfig {
    std::string project_name;
    std::string version = "1.0.0";
    std::string output_dir;
    std::vector<ExportTarget> targets;
    bool include_mods = true;
    bool compress_assets = true;
};

struct PackagingResult {
    bool success = false;
    std::string output_path;
    std::string message;
};

class ProjectPackager {
public:
    PackagingResult export_project(const PackagingConfig& config,
                                   std::string_view game_root) const;
    [[nodiscard]] std::string target_name(ExportTarget target) const;
};

} // namespace eve::content
