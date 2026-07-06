#include <eve/editor/wardrobe_tools.hpp>

namespace eve::editor {

void BodyRegionEditorPanel::set_volumes(const std::vector<human::BodyRegionVolume>& volumes) {
    volumes_ = volumes;
}

void BodyRegionEditorPanel::set_selected(human::BodyRegion region) {
    selected_ = region;
}

void ClothingItemEditorPanel::set_catalog(const std::vector<human::ClothingItemDefinition>& items) {
    items_ = items;
}

void ClothingItemEditorPanel::select_item(std::string_view item_id) {
    selected_item_id_ = std::string(item_id);
}

void OutfitBuilderPanel::set_wardrobe(const human::WardrobeSystem& wardrobe) {
    wardrobe_ = &wardrobe;
}

bool OutfitBuilderPanel::equip_item(std::string_view item_id) {
    if (wardrobe_ == nullptr) {
        return false;
    }
    if (!wardrobe_->can_equip(item_id, current_outfit_)) {
        return false;
    }
    current_outfit_ = wardrobe_->equip(item_id, current_outfit_);
    return true;
}

std::vector<std::string> OutfitBuilderPanel::clipping_warnings() const {
    return current_outfit_.clipping_warnings;
}

void InteractionGraphPanel::set_registry(const human::BodyInteractionRegistry& registry) {
    node_count_ = registry.definition_count() + registry.action_count();
}

void AnimationLayerDebuggerPanel::set_stack(const animation::AnimationLayerStack& stack) {
    active_layer_count_ = stack.active_layers().size();
}

void CameraPresetEditorPanel::set_camera(render::InspectionCamera& camera) {
    (void)camera;
}

void CameraPresetEditorPanel::add_preset(render::CameraPreset preset) {
    (void)preset;
    ++preset_count_;
}

void ClippingDebuggerPanel::set_warnings(const std::vector<std::string>& warnings) {
    warnings_ = warnings;
}

} // namespace eve::editor
