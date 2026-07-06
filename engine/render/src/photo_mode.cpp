#include <eve/core/filesystem/filesystem.hpp>
#include <eve/core/logging/logger.hpp>
#include <eve/render/photo_mode.hpp>

namespace eve::render {

PhotoMode::PhotoMode(InspectionCamera& camera, HumanShaderPipeline& pipeline)
    : camera_(camera)
    , pipeline_(pipeline) {}

void PhotoMode::enable(bool enabled) {
    enabled_ = enabled;
    if (enabled) {
        camera_.set_mode(CameraMode::Photo);
        camera_.depth_of_field_mutable().enabled = settings_.background_blur;
        camera_.exposure_mutable().hdr_enabled = true;
    }
}

void PhotoMode::set_settings(const PhotoModeSettings& settings) {
    settings_ = settings;
}

void PhotoMode::add_lighting_preset(PhotoModeLightingPreset preset) {
    lighting_presets_[preset.id] = std::move(preset);
}

void PhotoMode::capture_screenshot(std::string_view output_path) const {
    const std::string metadata =
        "EVE_HDR_SCREENSHOT scale=" + std::to_string(settings_.screenshot_scale) +
        " exposure=" + std::to_string(camera_.exposure().exposure);
    FileSystem::write_text_file(output_path, metadata);
    EVE_LOG(Info, "PhotoMode", "Captured HDR screenshot metadata to ", output_path);
}

} // namespace eve::render
