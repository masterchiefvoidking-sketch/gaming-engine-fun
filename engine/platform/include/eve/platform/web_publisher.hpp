#pragma once

#include <eve/platform/engine_mission.hpp>

#include <string>
#include <vector>

namespace eve::platform {

struct WebDeployConfig {
    std::string project_name;
    std::string output_dir = "dist/web";
    bool compress_assets = true;
    bool progressive_loading = true;
    bool enable_save_persistence = true;
    bool responsive_ui = true;
    bool mobile_friendly_controls = true;
    u32 target_fps = kTargetFpsWeb;
    std::string renderer = "webgpu";
    std::string wasm_output = "eve_game.wasm";
};

struct WebDeployResult {
    bool success = false;
    std::string output_path;
    std::string wasm_path;
    std::string manifest_path;
    std::string message;
};

class WebPublisher {
public:
    WebDeployResult publish(const WebDeployConfig& config, std::string_view game_root) const;
    bool generate_manifest(const WebDeployConfig& config, std::string_view output_dir) const;
    bool generate_loader_html(const WebDeployConfig& config, std::string_view output_dir) const;
};

} // namespace eve::platform
