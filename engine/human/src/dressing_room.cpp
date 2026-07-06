#include <eve/human/dressing_room.hpp>

namespace eve::human {

DressingRoomController::DressingRoomController(WardrobeSystem& wardrobe, CharacterFramework& character,
                                             BodyInteractionSystem& body_interactions)
    : wardrobe_(wardrobe), character_(character), body_interactions_(body_interactions) {}

bool DressingRoomController::execute(DressingRoomAction action, std::string_view parameter) {
    switch (action) {
    case DressingRoomAction::TryOnOutfit:
        return try_on(parameter);
    case DressingRoomAction::SaveOutfit:
        return save_current_outfit(parameter.empty() ? "custom_outfit" : parameter, "outfit_save.json");
    case DressingRoomAction::RandomizeOutfit: {
        const OutfitPreset random = wardrobe_.randomize_outfit();
        for (const std::string& item_id : random.item_ids) {
            try_on(item_id);
        }
        return true;
    }
    case DressingRoomAction::FavoriteOutfit: {
        OutfitPreset preset = wardrobe_.build_outfit_preset(
            parameter.empty() ? "favorite" : parameter, state_.current_outfit);
        preset.favorite = true;
        return wardrobe_.save_outfit(preset, "favorite_outfit.json");
    }
    case DressingRoomAction::CompareOutfits:
        state_.comparison_outfit = state_.current_outfit;
        return true;
    case DressingRoomAction::ChangeHairstyle:
        state_.active_hairstyle = std::string(parameter);
        character_.creator().profile_mutable().hair.style_id =
            parameter.empty() ? "long_straight" : std::string(parameter);
        return true;
    case DressingRoomAction::ApplyMakeup:
        state_.active_makeup = std::string(parameter);
        character_.creator().profile_mutable().face.makeup_intensity = 0.7f;
        return true;
    case DressingRoomAction::RemoveMakeup:
        state_.active_makeup.clear();
        character_.creator().profile_mutable().face.makeup_intensity = 0.0f;
        return true;
    case DressingRoomAction::ChangeJewelry:
    case DressingRoomAction::ChangeShoes:
        return try_on(parameter);
    case DressingRoomAction::PreviewLighting:
        character_.photo_mode().enable(true);
        character_.shader_pipeline().enable_bloom(true);
        character_.shader_pipeline().enable_ssao(true);
        return true;
    case DressingRoomAction::MirrorInspection:
        state_.mirror_active = true;
        character_.camera().set_mode(render::CameraMode::Mirror);
        return true;
    case DressingRoomAction::PhotoExport:
        character_.photo_mode().enable(true);
        character_.photo_mode().capture_screenshot(parameter.empty() ? "wardrobe_photo.hdr" : parameter);
        return true;
    case DressingRoomAction::PreviewWalk:
        character_.animation().set_initial(animation::AnimationStateId::Walk);
        return true;
    case DressingRoomAction::PreviewSit:
        character_.animation().set_initial(animation::AnimationStateId::Sit);
        return true;
    case DressingRoomAction::PreviewPose: {
        InteractionContextSnapshot context;
        context.room_id = state_.room_id;
        context.camera_mode = "wardrobe";
        body_interactions_.execute(parameter.empty() ? "pose_preview" : parameter, context);
        return true;
    }
    default:
        return false;
    }
}

bool DressingRoomController::try_on(std::string_view item_id) {
    if (!wardrobe_.can_equip(item_id, state_.current_outfit)) {
        return false;
    }
    state_.current_outfit = wardrobe_.equip(item_id, state_.current_outfit);
    return true;
}

bool DressingRoomController::save_current_outfit(std::string_view preset_id, std::string_view path) {
    const OutfitPreset preset = wardrobe_.build_outfit_preset(preset_id, state_.current_outfit);
    return wardrobe_.save_outfit(preset, path);
}

bool DressingRoomController::load_outfit_preset(std::string_view path) {
    OutfitPreset preset;
    if (!wardrobe_.load_outfit(path, preset)) {
        return false;
    }
    state_.current_outfit = {};
    for (const std::string& item_id : preset.item_ids) {
        try_on(item_id);
    }
    return true;
}

bool DressingRoomController::load_preset_by_id(std::string_view preset_id) {
    for (const OutfitPreset& preset : wardrobe_.saved_outfits()) {
        if (preset.id == preset_id) {
            state_.current_outfit = {};
            for (const std::string& item_id : preset.item_ids) {
                try_on(item_id);
            }
            return true;
        }
    }
    return false;
}

} // namespace eve::human
