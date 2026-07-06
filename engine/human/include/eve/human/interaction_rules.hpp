#pragma once

#include <eve/human/body_regions.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::human {

struct InteractionPermissionRequirements {
    f32 min_trust = 0.0f;
    f32 min_affection = 0.0f;
    f32 min_relationship = 0.0f;
    ConsentState required_consent = ConsentState::Granted;
    ContentRating max_rating = ContentRating::AdultsOnly;
    std::vector<std::string> required_outfit_tags;
    std::vector<std::string> blocked_outfit_tags;
    std::vector<std::string> allowed_rooms;
};

struct InteractionContextSnapshot {
    std::string room_id;
    std::string outfit_id;
    std::string mood = "neutral";
    f32 trust = 0.5f;
    f32 affection = 0.5f;
    f32 relationship = 0.5f;
    f32 privacy_level = 1.0f;
    f32 time_of_day = 12.0f;
    std::string active_animation_layer = "base";
    std::string camera_mode = "orbit";
    ContentRating content_rating = ContentRating::AdultsOnly;
    ConsentState consent = ConsentState::Granted;
    bool player_permission = true;
};

struct InteractionRuleResult {
    bool allowed = false;
    std::string reason;
};

class InteractionRuleEngine {
public:
    InteractionRuleResult evaluate(const InteractionPermissionRequirements& requirements,
                                   const InteractionContextSnapshot& context) const;
};

} // namespace eve::human
