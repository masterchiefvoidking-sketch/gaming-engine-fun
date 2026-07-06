#include <algorithm>
#include <eve/ai/boundaries.hpp>

namespace eve::ai {

ConsentCheckResult BoundaryConsentSystem::check_action(std::string_view action_id,
                                                       const CharacterSimState& character,
                                                       RelationshipStage stage) const {
    ConsentCheckResult result;
    if (!respects_boundary(character, action_id)) {
        result.reason = "Boundary violation";
        result.rejection_line = boundary_rejection_line(character, action_id);
        return result;
    }
    const auto it = std::find_if(character.boundaries.begin(), character.boundaries.end(),
                                 [action_id](const BoundaryRule& rule) {
                                     for (const std::string& blocked : rule.blocked_actions) {
                                         if (blocked == action_id) {
                                             return true;
                                         }
                                     }
                                     return false;
                                 });
    if (it != character.boundaries.end() &&
        minimum_stage_level(stage) < minimum_stage_level(it->min_stage)) {
        result.reason = "Relationship stage too low";
        result.rejection_line = "I don't think we're quite there yet...";
        return result;
    }
    const ConsentCheckResult consent = check_consent(character.consent, action_id);
    if (!consent.allowed) {
        return consent;
    }
    result.allowed = true;
    result.reason = "Allowed";
    return result;
}

ConsentCheckResult BoundaryConsentSystem::check_consent(ConsentState consent,
                                                        std::string_view action_id) const {
    ConsentCheckResult result;
    (void)action_id;
    if (consent == ConsentState::Denied || consent == ConsentState::Revoked) {
        result.reason = "Consent not granted";
        result.rejection_line = "I'd rather not right now.";
        return result;
    }
    result.allowed = true;
    return result;
}

bool BoundaryConsentSystem::respects_boundary(const CharacterSimState& character,
                                              std::string_view action_id) const {
    for (const BoundaryRule& rule : character.boundaries) {
        for (const std::string& blocked : rule.blocked_actions) {
            if (blocked == action_id) {
                return false;
            }
        }
    }
    return true;
}

std::string BoundaryConsentSystem::boundary_rejection_line(const CharacterSimState& character,
                                                           std::string_view action_id) const {
    for (const BoundaryRule& rule : character.boundaries) {
        for (const std::string& blocked : rule.blocked_actions) {
            if (blocked == action_id) {
                return "Please respect my boundary: " + rule.description;
            }
        }
    }
    return "I'm not comfortable with that.";
}

} // namespace eve::ai
