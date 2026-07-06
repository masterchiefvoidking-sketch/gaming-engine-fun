#pragma once

#include <eve/human/body_interaction_system.hpp>
#include <eve/human/body_regions.hpp>
#include <eve/human/wardrobe_system.hpp>
#include <eve/animation/layer_stack.hpp>
#include <eve/render/inspection_camera.hpp>

#include <string>
#include <vector>

namespace eve::editor {

class BodyRegionEditorPanel {
public:
    void set_volumes(const std::vector<human::BodyRegionVolume>& volumes);
    void set_selected(human::BodyRegion region);
    [[nodiscard]] bool debug_visualization() const { return debug_visualization_; }
    void set_debug_visualization(bool enabled) { debug_visualization_ = enabled; }

private:
    std::vector<human::BodyRegionVolume> volumes_;
    human::BodyRegion selected_ = human::BodyRegion::Torso;
    bool debug_visualization_ = false;
};

class ClothingItemEditorPanel {
public:
    void set_catalog(const std::vector<human::ClothingItemDefinition>& items);
    [[nodiscard]] const std::string& selected_item_id() const { return selected_item_id_; }
    void select_item(std::string_view item_id);

private:
    std::vector<human::ClothingItemDefinition> items_;
    std::string selected_item_id_;
};

class OutfitBuilderPanel {
public:
    void set_wardrobe(const human::WardrobeSystem& wardrobe);
    [[nodiscard]] human::LayeredOutfit current_outfit() const { return current_outfit_; }
    bool equip_item(std::string_view item_id);
    [[nodiscard]] std::vector<std::string> clipping_warnings() const;

private:
    const human::WardrobeSystem* wardrobe_ = nullptr;
    human::LayeredOutfit current_outfit_;
};

class WardrobeRoomPreviewPanel {
public:
    void set_room_id(std::string_view room_id) { room_id_ = std::string(room_id); }
    [[nodiscard]] const std::string& room_id() const { return room_id_; }

private:
    std::string room_id_ = "wardrobe";
};

class InteractionGraphPanel {
public:
    void set_registry(const human::BodyInteractionRegistry& registry);
    [[nodiscard]] std::size_t node_count() const { return node_count_; }

private:
    std::size_t node_count_ = 0;
};

class RelationshipReactionEditorPanel {
public:
    void set_reaction_count(std::size_t count) { reaction_count_ = count; }
    [[nodiscard]] std::size_t reaction_count() const { return reaction_count_; }

private:
    std::size_t reaction_count_ = 0;
};

class AnimationLayerDebuggerPanel {
public:
    void set_stack(const animation::AnimationLayerStack& stack);
    [[nodiscard]] std::size_t active_layer_count() const { return active_layer_count_; }

private:
    std::size_t active_layer_count_ = 0;
};

class CameraPresetEditorPanel {
public:
    void set_camera(render::InspectionCamera& camera);
    void add_preset(render::CameraPreset preset);
    [[nodiscard]] std::size_t preset_count() const { return preset_count_; }

private:
    std::size_t preset_count_ = 0;
};

class MaterialPreviewPanel {
public:
    void set_material_path(std::string_view path) { material_path_ = std::string(path); }
    [[nodiscard]] const std::string& material_path() const { return material_path_; }

private:
    std::string material_path_;
};

class MirrorPreviewPanel {
public:
    void set_mirror_active(bool active) { mirror_active_ = active; }
    [[nodiscard]] bool mirror_active() const { return mirror_active_; }

private:
    bool mirror_active_ = false;
};

class ClippingDebuggerPanel {
public:
    void set_warnings(const std::vector<std::string>& warnings);
    [[nodiscard]] const std::vector<std::string>& warnings() const { return warnings_; }

private:
    std::vector<std::string> warnings_;
};

} // namespace eve::editor
