#include <eve/human/interaction_rules.hpp>

namespace eve::human {

InteractionRuleResult InteractionRuleEngine::evaluate(const InteractionPermissionRequirements& requirements,
                                                      const InteractionContextSnapshot& context) const {
    InteractionRuleResult result;
    if (!context.player_permission) {
        result.reason = "Player lacks permission";
        return result;
    }
    if (context.consent == ConsentState::Denied || context.consent == ConsentState::Revoked) {
        result.reason = "Consent not granted";
        return result;
    }
    if (requirements.required_consent == ConsentState::Granted && context.consent != ConsentState::Granted) {
        result.reason = "Explicit consent required";
        return result;
    }
    if (context.trust < requirements.min_trust) {
        result.reason = "Insufficient trust";
        return result;
    }
    if (context.affection < requirements.min_affection) {
        result.reason = "Insufficient affection";
        return result;
    }
    if (context.relationship < requirements.min_relationship) {
        result.reason = "Insufficient relationship level";
        return result;
    }
    if (context.content_rating > requirements.max_rating) {
        result.reason = "Content rating lockout";
        return result;
    }
    if (!requirements.allowed_rooms.empty()) {
        bool room_ok = false;
        for (const std::string& room : requirements.allowed_rooms) {
            if (room == context.room_id) {
                room_ok = true;
                break;
            }
        }
        if (!room_ok) {
            result.reason = "Action not allowed in this room";
            return result;
        }
    }
    result.allowed = true;
    result.reason = "Allowed";
    return result;
}

} // namespace eve::human
