#pragma once

#include <eve/human/body_interaction_system.hpp>
#include <eve/human/body_regions.hpp>
#include <eve/human/character_framework.hpp>
#include <eve/human/dressing_room.hpp>
#include <eve/human/wardrobe_system.hpp>
#include <eve/ai/romance_hooks.hpp>
#include <eve/animation/layer_stack.hpp>
#include <eve/render/mirror_renderer.hpp>

#include <string>
#include <vector>

namespace eve::wardrobe_slice {

class WardrobeVerticalSlice {
public:
    WardrobeVerticalSlice();

    bool initialize(std::string_view data_root);
    void shutdown();
    void update(f32 delta_seconds);

    void orbit_camera(f32 yaw_delta, f32 pitch_delta, f32 zoom_delta);
    void pan_camera(f32 x_delta, f32 y_delta);
    bool select_body_region(const math::Vec3& ray_origin, const math::Vec3& ray_direction);
    bool change_outfit(std::string_view item_id);
    bool load_outfit_preset(std::string_view preset_id);
    bool save_outfit_preset(std::string_view preset_id);
    void preview_animation(std::string_view animation_id);
    void mirror_inspection();
    bool trigger_romance_reaction(std::string_view reaction_id);
    bool capture_photo(std::string_view path);

    [[nodiscard]] const human::DressingRoomState& dressing_state() const { return dressing_room_.state(); }
    [[nodiscard]] human::BodyRegion selected_region() const { return selected_region_; }

private:
    void load_camera_presets(std::string_view data_root);
    void setup_default_body_volumes();

    human::CharacterFramework character_;
    human::WardrobeSystem wardrobe_;
    human::BodyInteractionRegistry interaction_registry_;
    human::BodyInteractionSystem body_interactions_;
    human::DressingRoomController dressing_room_;
    ai::RomanceSimulationHooks romance_;
    ai::RelationshipRegistry relationships_;
    animation::AnimationLayerStack animation_layers_;
    render::MirrorRenderer mirror_;

    std::vector<human::BodyRegionVolume> body_volumes_;
    human::BodyRegion selected_region_ = human::BodyRegion::Torso;
    std::string data_root_;
    bool initialized_ = false;
};

} // namespace eve::wardrobe_slice
