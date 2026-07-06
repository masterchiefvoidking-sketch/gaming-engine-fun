#include <eve/platform/web_publisher.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::platform {

WebDeployResult WebPublisher::publish(const WebDeployConfig& config,
                                      std::string_view game_root) const {
    WebDeployResult result;
    if (!FileSystem::exists(game_root)) {
        result.message = "Game root not found";
        return result;
    }

    const std::string out_dir = config.output_dir;
    FileSystem::create_directories(out_dir);
    FileSystem::create_directories(out_dir + "/assets");

    if (!generate_manifest(config, out_dir)) {
        result.message = "Failed to generate manifest";
        return result;
    }
    if (!generate_loader_html(config, out_dir)) {
        result.message = "Failed to generate loader";
        return result;
    }

    nlohmann::json build_info;
    build_info["platform"] = "web";
    build_info["renderer"] = config.renderer;
    build_info["wasm"] = config.wasm_output;
    build_info["target_fps"] = config.target_fps;
    build_info["compress_assets"] = config.compress_assets;
    build_info["progressive_loading"] = config.progressive_loading;
    build_info["save_persistence"] = config.enable_save_persistence;
    build_info["game_root"] = std::string(game_root);
    build_info["note"] = "WebAssembly build requires Emscripten toolchain (future CI step)";
    FileSystem::write_text_file(out_dir + "/build_info.json", build_info.dump(2));

    result.success = true;
    result.output_path = out_dir;
    result.wasm_path = out_dir + "/" + config.wasm_output;
    result.manifest_path = out_dir + "/manifest.json";
    result.message = "Web deployment package generated";
    return result;
}

bool WebPublisher::generate_manifest(const WebDeployConfig& config,
                                     std::string_view output_dir) const {
    nlohmann::json manifest;
    manifest["name"] = config.project_name;
    manifest["renderer"] = config.renderer;
    manifest["wasm"] = config.wasm_output;
    manifest["assets"] = {{"compression", config.compress_assets ? "brotli" : "none"},
                          {"progressive", config.progressive_loading}};
    manifest["features"] = {{"save_persistence", config.enable_save_persistence},
                            {"responsive_ui", config.responsive_ui},
                            {"touch_controls", config.mobile_friendly_controls}};
    return FileSystem::write_text_file(std::string(output_dir) + "/manifest.json",
                                       manifest.dump(2));
}

bool WebPublisher::generate_loader_html(const WebDeployConfig& config,
                                        std::string_view output_dir) const {
    const std::string html = R"(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>)" + config.project_name +
                             R"(</title>
  <style>body{margin:0;background:#1a1a2e;color:#eee;font-family:sans-serif}
  #canvas{width:100vw;height:100vh;display:block}</style>
</head>
<body>
  <canvas id="canvas"></canvas>
  <script type="module">
    // WebGPU + WebAssembly loader placeholder
    const manifest = await fetch('manifest.json').then(r => r.json());
    console.log('EVE Web:', manifest.name, 'renderer:', manifest.renderer);
  </script>
</body>
</html>)";
    return FileSystem::write_text_file(std::string(output_dir) + "/index.html", html);
}

} // namespace eve::platform
