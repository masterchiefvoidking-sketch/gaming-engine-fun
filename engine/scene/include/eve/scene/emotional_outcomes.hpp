#pragma once

#include <eve/ai/character_sim_state.hpp>
#include <eve/ai/memory_system.hpp>
#include <eve/ai/relationship.hpp>
#include <eve/scene/scene_types.hpp>

#include <string>
#include <unordered_map>

namespace eve::scene {

struct OutcomeApplicationResult {
    bool applied = false;
    std::string memory_id;
    std::string unlock_dialogue_id;
    std::string unlock_event_id;
    f32 cooldown_hours = 0.0f;
};

class EmotionalOutcomeSystem {
public:
    bool load_outcomes(std::string_view path);
    void register_outcome(EmotionalOutcomeDefinition outcome);

    OutcomeApplicationResult apply(std::string_view outcome_id, ai::CharacterSimState& character,
                                   ai::RelationshipModel& relationship,
                                   const ai::MemorySystem& memories, u32 day_index) const;

    [[nodiscard]] std::size_t outcome_count() const { return outcomes_.size(); }

private:
    std::unordered_map<std::string, EmotionalOutcomeDefinition> outcomes_;
};

} // namespace eve::scene
