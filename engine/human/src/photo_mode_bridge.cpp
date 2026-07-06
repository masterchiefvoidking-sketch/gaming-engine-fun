#include <eve/human/photo_mode_bridge.hpp>

namespace eve::human {

void apply_photo_expression_preset(CharacterCreator& creator, std::string_view preset_id) {
    if (preset_id == "smile") {
        creator.set_morph("mouth_smile", 0.8f);
        creator.set_morph("cheek_raise", 0.4f);
    } else if (preset_id == "serious") {
        creator.set_morph("brow_lower", 0.6f);
        creator.set_morph("mouth_smile", 0.0f);
    } else {
        creator.set_morph("mouth_smile", 0.1f);
        creator.set_morph("brow_lower", 0.0f);
    }
}

} // namespace eve::human
