#include <eve/editor/anime_visual_tools.hpp>

namespace eve::editor {

void AnimeMaterialEditorPanel::set_material(render::AnimeHumanMaterial& material) {
    material_ = &material;
    blush_ = material.skin().blush_intensity;
}

void AnimeMaterialEditorPanel::set_blush(f32 intensity) {
    blush_ = intensity;
    if (material_ != nullptr) {
        material_->set_blush(intensity);
    }
}

void ToonRampEditorPanel::set_material(render::AnimeHumanMaterial& material) {
    ramp_stop_count_ = material.toon_ramp().size();
}

void OutlineEditorPanel::set_thickness(f32 thickness) {
    thickness_ = thickness;
}

void EyeHighlightEditorPanel::set_eye_system(render::AnimeEyeSystem& eyes) {
    highlight_count_ = eyes.active_preset().highlights.size();
}

void ExpressionPresetEditorPanel::set_system(const human::AnimeExpressionSystem& expressions) {
    preset_count_ = 0;
    for (const char* id :
         {"neutral", "smile", "soft_smile", "blushing", "shy", "flirty", "romantic_gaze"}) {
        if (expressions.find(id) != nullptr) {
            ++preset_count_;
        }
    }
}

void RoomLightingEditorPanel::set_lighting(const world::AnimeRoomLightingSystem& lighting) {
    active_id_ = lighting.active().id;
}

void RomanceCameraEditorPanel::set_controller(render::RomanceCameraController& camera) {
    rule_of_thirds_ = camera.composition_guide().enabled;
}

} // namespace eve::editor
