#include <algorithm>
#include <eve/scene/emotional_outcomes.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::scene {

bool EmotionalOutcomeSystem::load_outcomes(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            EmotionalOutcomeDefinition outcome;
            outcome.id = entry.value("id", "");
            outcome.label = entry.value("label", "");
            outcome.trust_delta = entry.value("trust_delta", 0.0f);
            outcome.affection_delta = entry.value("affection_delta", 0.0f);
            outcome.respect_delta = entry.value("respect_delta", 0.0f);
            outcome.comfort_delta = entry.value("comfort_delta", 0.0f);
            outcome.confidence_delta = entry.value("confidence_delta", 0.0f);
            outcome.stress_delta = entry.value("stress_delta", 0.0f);
            outcome.memory_template_id = entry.value("memory_template", "");
            outcome.unlock_dialogue_id = entry.value("unlock_dialogue", "");
            outcome.unlock_event_id = entry.value("unlock_event", "");
            outcome.cooldown_hours = entry.value("cooldown_hours", 0.0f);
            register_outcome(std::move(outcome));
        }
        return !outcomes_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void EmotionalOutcomeSystem::register_outcome(EmotionalOutcomeDefinition outcome) {
    outcomes_[outcome.id] = std::move(outcome);
}

OutcomeApplicationResult EmotionalOutcomeSystem::apply(
    std::string_view outcome_id, ai::CharacterSimState& character,
    ai::RelationshipModel& relationship, const ai::MemorySystem& memories, u32 day_index) const {
    OutcomeApplicationResult result;
    const auto it = outcomes_.find(std::string(outcome_id));
    if (it == outcomes_.end()) {
        return result;
    }
    const EmotionalOutcomeDefinition& outcome = it->second;

    character.trust = std::clamp(character.trust + outcome.trust_delta, 0.0f, 1.0f);
    character.affection = std::clamp(character.affection + outcome.affection_delta, 0.0f, 1.0f);
    character.respect = std::clamp(character.respect + outcome.respect_delta, 0.0f, 1.0f);
    character.comfort = std::clamp(character.comfort + outcome.comfort_delta, 0.0f, 1.0f);
    character.confidence = std::clamp(character.confidence + outcome.confidence_delta, 0.0f, 1.0f);
    character.stress = std::clamp(character.stress + outcome.stress_delta, 0.0f, 1.0f);
    relationship.adjust_scores(outcome.trust_delta, outcome.affection_delta, outcome.respect_delta,
                             0.0f);

    if (!outcome.memory_template_id.empty()) {
        ai::MemoryEntry mem =
            memories.create_memory(outcome.memory_template_id, "player", day_index);
        relationship.add_memory(std::move(mem));
        result.memory_id = outcome.memory_template_id;
    }

    result.applied = true;
    result.unlock_dialogue_id = outcome.unlock_dialogue_id;
    result.unlock_event_id = outcome.unlock_event_id;
    result.cooldown_hours = outcome.cooldown_hours;
    return result;
}

} // namespace eve::scene
