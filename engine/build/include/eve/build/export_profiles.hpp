#pragma once

#include <eve/core/types.hpp>

#include <string>

namespace eve::build {

enum class ExportProfile : u8 {
    WindowsDevelopment = 0,
    WindowsRelease,
    WebDevelopment,
    WebRelease,
};

enum class PlatformTarget : u8 {
    Windows = 0,
    Web,
    AndroidFuture,
    IosFuture,
};

struct ExportProfileSettings {
    PlatformTarget platform = PlatformTarget::Windows;
    std::string renderer = "vulkan";
    bool compress_assets = false;
    u32 max_texture_size = 4096;
    u32 shader_quality = 3;
    u32 hair_quality = 3;
    u32 cloth_quality = 2;
    bool screenshot_support = true;
    bool debug_console = true;
    std::string save_location = "local";
    bool progressive_loading = false;
    bool touch_controls = false;
    bool offline_only = true;
};

[[nodiscard]] ExportProfileSettings settings_for_profile(ExportProfile profile);
[[nodiscard]] const char* profile_name(ExportProfile profile);
[[nodiscard]] const char* platform_name(PlatformTarget platform);

} // namespace eve::build
