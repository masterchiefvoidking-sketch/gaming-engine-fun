#pragma once

#include <eve/human/body_interaction_system.hpp>
#include <eve/human/character_framework.hpp>
#include <eve/human/wardrobe_system.hpp>

#include <string>

namespace eve::human {

enum class DressingRoomAction : u8 {
    TryOnOutfit,
    SaveOutfit,
    FavoriteOutfit,
    RandomizeOutfit,
    CompareOutfits,
    ChangeHairstyle,
    ApplyMakeup,
    RemoveMakeup,
    ChangeJewelry,
    ChangeShoes,
    PreviewPose,
    PreviewWalk,
    PreviewSit,
    PreviewLighting,
    MirrorInspection,
    PhotoExport,
};

struct DressingRoomState {
    std::string room_id = "wardrobe";
    LayeredOutfit current_outfit;
    LayeredOutfit comparison_outfit;
    std::string active_hairstyle;
    std::string active_makeup;
    bool mirror_active = false;
};

class DressingRoomController {
public:
    DressingRoomController(WardrobeSystem& wardrobe, CharacterFramework& character,
                           BodyInteractionSystem& body_interactions);

    bool execute(DressingRoomAction action, std::string_view parameter = {});
    [[nodiscard]] const DressingRoomState& state() const { return state_; }
    [[nodiscard]] const WardrobeSystem& wardrobe() const { return wardrobe_; }

    bool try_on(std::string_view item_id);
    bool save_current_outfit(std::string_view preset_id, std::string_view path);
    bool load_outfit_preset(std::string_view path);
    bool load_preset_by_id(std::string_view preset_id);

private:
    WardrobeSystem& wardrobe_;
    CharacterFramework& character_;
    BodyInteractionSystem& body_interactions_;
    DressingRoomState state_;
};

} // namespace eve::human
