#pragma once

#include <eve/ai/character_sim_state.hpp>
#include <eve/ai/relationship_stages.hpp>

#include <string>

namespace eve::ai {

struct ConsentCheckResult {
    bool allowed = false;
    std::string reason;
    std::string rejection_line;
};

class BoundaryConsentSystem {
public:
    ConsentCheckResult check_action(std::string_view action_id, const CharacterSimState& character,
                                    RelationshipStage stage) const;
    ConsentCheckResult check_consent(ConsentState consent, std::string_view action_id) const;
    bool respects_boundary(const CharacterSimState& character, std::string_view action_id) const;
    [[nodiscard]] std::string boundary_rejection_line(const CharacterSimState& character,
                                                      std::string_view action_id) const;
};

} // namespace eve::ai
