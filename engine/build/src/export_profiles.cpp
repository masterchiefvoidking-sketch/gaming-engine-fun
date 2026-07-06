#include <eve/build/export_profiles.hpp>

namespace eve::build {

ExportProfileSettings settings_for_profile(ExportProfile profile) {
    ExportProfileSettings settings;
    switch (profile) {
    case ExportProfile::WindowsDevelopment:
        settings.platform = PlatformTarget::Windows;
        settings.renderer = "vulkan";
        settings.compress_assets = false;
        settings.max_texture_size = 4096;
        settings.shader_quality = 3;
        settings.hair_quality = 3;
        settings.cloth_quality = 3;
        settings.screenshot_support = true;
        settings.debug_console = true;
        settings.save_location = "local_files";
        settings.progressive_loading = false;
        settings.touch_controls = false;
        break;
    case ExportProfile::WindowsRelease:
        settings.platform = PlatformTarget::Windows;
        settings.renderer = "vulkan";
        settings.compress_assets = true;
        settings.max_texture_size = 4096;
        settings.shader_quality = 3;
        settings.hair_quality = 3;
        settings.cloth_quality = 3;
        settings.screenshot_support = true;
        settings.debug_console = false;
        settings.save_location = "local_files";
        break;
    case ExportProfile::WebDevelopment:
        settings.platform = PlatformTarget::Web;
        settings.renderer = "webgpu";
        settings.compress_assets = false;
        settings.max_texture_size = 2048;
        settings.shader_quality = 2;
        settings.hair_quality = 2;
        settings.cloth_quality = 1;
        settings.screenshot_support = true;
        settings.debug_console = true;
        settings.save_location = "indexeddb";
        settings.progressive_loading = true;
        settings.touch_controls = true;
        break;
    case ExportProfile::WebRelease:
        settings.platform = PlatformTarget::Web;
        settings.renderer = "webgpu";
        settings.compress_assets = true;
        settings.max_texture_size = 1024;
        settings.shader_quality = 1;
        settings.hair_quality = 1;
        settings.cloth_quality = 1;
        settings.screenshot_support = true;
        settings.debug_console = false;
        settings.save_location = "indexeddb";
        settings.progressive_loading = true;
        settings.touch_controls = true;
        break;
    }
    settings.offline_only = true;
    return settings;
}

const char* profile_name(ExportProfile profile) {
    switch (profile) {
    case ExportProfile::WindowsRelease:
        return "windows_release";
    case ExportProfile::WebDevelopment:
        return "web_development";
    case ExportProfile::WebRelease:
        return "web_release";
    default:
        return "windows_development";
    }
}

const char* platform_name(PlatformTarget platform) {
    switch (platform) {
    case PlatformTarget::Web:
        return "web";
    case PlatformTarget::AndroidFuture:
        return "android";
    case PlatformTarget::IosFuture:
        return "ios";
    default:
        return "windows";
    }
}

} // namespace eve::build
