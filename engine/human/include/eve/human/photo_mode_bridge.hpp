#pragma once

#include <eve/human/character_profile.hpp>
#include <string_view>

namespace eve::human {

void apply_photo_expression_preset(CharacterCreator& creator, std::string_view preset_id);

} // namespace eve::human
