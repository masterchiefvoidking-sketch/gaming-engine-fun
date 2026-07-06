#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::product {

struct ContentCompletionReport {
    bool complete = false;
    u32 wardrobe_items = 0;
    u32 hairstyles = 0;
    u32 expressions = 0;
    u32 rooms = 0;
    u32 object_interactions = 0;
    u32 dialogue_lines = 0;
    u32 events = 0;
    u32 camera_presets = 0;
    u32 lighting_presets = 0;
    u32 photo_presets = 0;
    std::vector<std::string> missing_requirements;
};

class ContentCompletionValidator {
public:
    ContentCompletionReport validate(std::string_view game_root,
                                     std::string_view data_root) const;
};

} // namespace eve::product
