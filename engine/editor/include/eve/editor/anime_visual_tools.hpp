#pragma once

#include <eve/human/anime_expression_system.hpp>
#include <eve/render/anime_eye_system.hpp>
#include <eve/render/anime_human_material.hpp>
#include <eve/render/romance_camera.hpp>
#include <eve/world/anime_room_lighting.hpp>

namespace eve::editor {

class AnimeMaterialEditorPanel {
public:
    void set_material(render::AnimeHumanMaterial& material);
    void set_blush(f32 intensity);
    [[nodiscard]] f32 blush() const { return blush_; }

private:
    render::AnimeHumanMaterial* material_ = nullptr;
    f32 blush_ = 0.0f;
};

class ToonRampEditorPanel {
public:
    void set_material(render::AnimeHumanMaterial& material);
    [[nodiscard]] std::size_t ramp_stop_count() const { return ramp_stop_count_; }

private:
    std::size_t ramp_stop_count_ = 0;
};

class OutlineEditorPanel {
public:
    void set_thickness(f32 thickness);
    [[nodiscard]] f32 thickness() const { return thickness_; }

private:
    f32 thickness_ = 1.2f;
};

class EyeHighlightEditorPanel {
public:
    void set_eye_system(render::AnimeEyeSystem& eyes);
    [[nodiscard]] std::size_t highlight_count() const { return highlight_count_; }

private:
    std::size_t highlight_count_ = 0;
};

class HairHighlightEditorPanel {
public:
    void set_highlight_band_count(std::size_t count) { band_count_ = count; }
    [[nodiscard]] std::size_t band_count() const { return band_count_; }

private:
    std::size_t band_count_ = 0;
};

class BlushEditorPanel {
public:
    void set_intensity(f32 intensity) { intensity_ = intensity; }
    [[nodiscard]] f32 intensity() const { return intensity_; }

private:
    f32 intensity_ = 0.0f;
};

class ExpressionPresetEditorPanel {
public:
    void set_system(const human::AnimeExpressionSystem& expressions);
    [[nodiscard]] std::size_t preset_count() const { return preset_count_; }

private:
    std::size_t preset_count_ = 0;
};

class RoomLightingEditorPanel {
public:
    void set_lighting(const world::AnimeRoomLightingSystem& lighting);
    [[nodiscard]] const std::string& active_preset_id() const { return active_id_; }

private:
    std::string active_id_;
};

class RomanceCameraEditorPanel {
public:
    void set_controller(render::RomanceCameraController& camera);
    [[nodiscard]] bool rule_of_thirds() const { return rule_of_thirds_; }

private:
    bool rule_of_thirds_ = false;
};

class ScreenshotStudioPanel {
public:
    void set_output_path(std::string_view path) { output_path_ = std::string(path); }
    [[nodiscard]] const std::string& output_path() const { return output_path_; }

private:
    std::string output_path_ = "anime_screenshot.hdr";
};

} // namespace eve::editor
