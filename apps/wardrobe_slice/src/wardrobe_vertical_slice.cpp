#include <eve/wardrobe_slice/wardrobe_vertical_slice.hpp>

#include <eve/core/filesystem/filesystem.hpp>
#include <eve/core/logging/logger.hpp>

#include <nlohmann/json.hpp>

namespace eve::wardrobe_slice {

WardrobeVerticalSlice::WardrobeVerticalSlice()
    : body_interactions_(interaction_registry_)
    , dressing_room_(wardrobe_, character_, body_interactions_) {}

bool WardrobeVerticalSlice::initialize(std::string_view data_root) {
    data_root_ = std::string(data_root);
    if (!character_.initialize()) {
        return false;
    }

    const std::string wardrobe_path = data_root_ + "/wardrobe/items.json";
    const std::string outfits_path = data_root_ + "/wardrobe/outfits.json";
    const std::string body_path = data_root_ + "/characters/default/body_definition.json";
    const std::string preset_path = data_root_ + "/characters/default/female_preset.json";
    const std::string interactions_path = data_root_ + "/interactions/body_regions.json";
    const std::string romance_path = data_root_ + "/romance/reactions.json";
    const std::string layers_path = data_root_ + "/animation/layers.json";

    if (!wardrobe_.load_items(wardrobe_path)) {
        EVE_LOG(Warn, "WardrobeSlice", "Could not load wardrobe items from ", wardrobe_path);
    }
    wardrobe_.load_outfits(outfits_path);
    interaction_registry_.load_from_file(interactions_path);
    romance_.load_reactions(romance_path);
    animation_layers_.load_definitions(layers_path);

    character_.creator().load_from_file(preset_path);
    character_.camera().set_mode(render::CameraMode::Wardrobe);

    setup_default_body_volumes();
    load_body_volumes_from_file(body_path, body_volumes_);

    load_camera_presets(data_root);
    mirror_.add_mirror({"wardrobe_mirror", {0.0f, 1.2f, -1.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 2.0f}});

    dressing_room_.load_preset_by_id("casual_day");
    initialized_ = true;
    EVE_LOG(Info, "WardrobeSlice", "Wardrobe vertical slice initialized");
    return true;
}

void WardrobeVerticalSlice::shutdown() {
    if (initialized_) {
        character_.shutdown();
        initialized_ = false;
    }
}

void WardrobeVerticalSlice::update(f32 delta_seconds) {
    character_.update(delta_seconds);
    animation_layers_.update(delta_seconds);
}

void WardrobeVerticalSlice::orbit_camera(f32 yaw_delta, f32 pitch_delta, f32 zoom_delta) {
    render::OrbitCameraInput input{};
    input.orbit_delta_yaw = yaw_delta;
    input.orbit_delta_pitch = pitch_delta;
    input.zoom_delta = zoom_delta;
    character_.camera().update(0.0f, input);
}

void WardrobeVerticalSlice::pan_camera(f32 x_delta, f32 y_delta) {
    render::OrbitCameraInput input{};
    input.pan_delta = {x_delta, y_delta};
    character_.camera().update(0.0f, input);
}

bool WardrobeVerticalSlice::select_body_region(const math::Vec3& ray_origin,
                                               const math::Vec3& ray_direction) {
    const human::BodyRegionTarget target =
        body_interactions_.query_region(body_volumes_, ray_origin, ray_direction);
    if (target.hovered) {
        selected_region_ = target.region;
        for (const human::BodyRegionVolume& volume : body_volumes_) {
            if (volume.region == target.region) {
                character_.camera().focus_body_region(volume.center, volume.focus_offset);
                character_.camera().transition_to(volume.center + math::Vec3{0.0f, 0.0f, 1.2f},
                                                  volume.center, 0.5f);
                break;
            }
        }
        return true;
    }
    return false;
}

bool WardrobeVerticalSlice::change_outfit(std::string_view item_id) {
    return dressing_room_.try_on(item_id);
}

bool WardrobeVerticalSlice::load_outfit_preset(std::string_view preset_id) {
    return dressing_room_.load_preset_by_id(preset_id);
}

bool WardrobeVerticalSlice::save_outfit_preset(std::string_view preset_id) {
    const std::string path = data_root_ + "/wardrobe/saved_" + std::string(preset_id) + ".json";
    return dressing_room_.save_current_outfit(preset_id, path);
}

void WardrobeVerticalSlice::preview_animation(std::string_view animation_id) {
    if (animation_id == "walk") {
        dressing_room_.execute(human::DressingRoomAction::PreviewWalk);
    } else if (animation_id == "sit") {
        dressing_room_.execute(human::DressingRoomAction::PreviewSit);
    } else {
        dressing_room_.execute(human::DressingRoomAction::PreviewPose, animation_id);
    }
    animation_layers_.set_layer_clip(animation::AnimationLayerId::BaseLocomotion, std::string(animation_id));
}

void WardrobeVerticalSlice::mirror_inspection() {
    dressing_room_.execute(human::DressingRoomAction::MirrorInspection);
    mirror_.set_active_mirror("wardrobe_mirror");
    character_.camera().apply_preset("mirror_front");
}

bool WardrobeVerticalSlice::trigger_romance_reaction(std::string_view reaction_id) {
    ai::RomanceContext context;
    context.room_id = "wardrobe";
    context.trust = 0.6f;
    context.affection = 0.5f;
    context.consent_granted = true;
    const ai::RomanceReactionResult result = romance_.trigger(reaction_id, context, relationships_);
    return result.accepted;
}

bool WardrobeVerticalSlice::capture_photo(std::string_view path) {
    return dressing_room_.execute(human::DressingRoomAction::PhotoExport, path);
}

void WardrobeVerticalSlice::load_camera_presets(std::string_view data_root) {
    const std::string path = std::string(data_root) + "/camera/wardrobe_presets.json";
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            render::CameraPreset preset;
            preset.id = entry.value("id", "");
            preset.label = entry.value("label", preset.id);
            preset.fov_y_degrees = entry.value("fov", 35.0f);
            if (entry.contains("position")) {
                preset.position.x = entry["position"].value("x", 0.0f);
                preset.position.y = entry["position"].value("y", 1.5f);
                preset.position.z = entry["position"].value("z", 3.0f);
            }
            if (entry.contains("focus")) {
                preset.focus.x = entry["focus"].value("x", 0.0f);
                preset.focus.y = entry["focus"].value("y", 1.0f);
                preset.focus.z = entry["focus"].value("z", 0.0f);
            }
            character_.camera().add_preset(std::move(preset));
        }
    } catch (const nlohmann::json::exception&) {
    }
}

void WardrobeVerticalSlice::setup_default_body_volumes() {
    human::BodyRegionVolume head;
    head.id = "head_vol";
    head.region = human::BodyRegion::Head;
    head.center = {0.0f, 1.65f, 0.0f};
    head.half_extents = {0.12f, 0.14f, 0.12f};
    body_volumes_.push_back(head);

    human::BodyRegionVolume torso;
    torso.id = "torso_vol";
    torso.region = human::BodyRegion::Torso;
    torso.center = {0.0f, 1.15f, 0.0f};
    torso.half_extents = {0.18f, 0.22f, 0.12f};
    body_volumes_.push_back(torso);

    human::BodyRegionVolume hips;
    hips.id = "hips_vol";
    hips.region = human::BodyRegion::Hips;
    hips.center = {0.0f, 0.82f, 0.0f};
    hips.half_extents = {0.20f, 0.10f, 0.14f};
    body_volumes_.push_back(hips);
}

} // namespace eve::wardrobe_slice
