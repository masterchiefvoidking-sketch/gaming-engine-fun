#pragma once

#include <eve/ai/character_sim_state.hpp>
#include <eve/ai/relationship_stages.hpp>
#include <eve/ai/wardrobe.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::ai {

struct DialogueChoiceEffect {
    f32 trust_delta = 0.0f;
    f32 affection_delta = 0.0f;
    f32 comfort_delta = 0.0f;
    std::string memory_tag;
    std::string next_node_id;
};

struct DialogueChoice {
    std::string id;
    std::string label;
    DialogueChoiceEffect effect;
};

struct DialogueConditions {
    RelationshipStage min_stage = RelationshipStage::Stranger;
    Mood required_mood = Mood::Neutral;
    bool mood_required = false;
    std::vector<std::string> required_rooms;
    std::vector<std::string> required_outfit_tags;
    std::vector<std::string> required_memories;
    std::vector<std::string> time_of_day; // morning, afternoon, evening, night
    f32 min_trust = 0.0f;
    f32 min_affection = 0.0f;
};

struct DialogueNode {
    std::string id;
    std::string speaker;
    std::string line;
    std::string category; // compliment, gift, date, apology, routine, memory_callback
    DialogueConditions conditions;
    std::vector<DialogueChoice> choices;
    std::string default_next;
};

struct DialogueContext {
    CharacterSimState* character = nullptr;
    RelationshipStage stage = RelationshipStage::Stranger;
    f32 time_of_day = 18.0f;
    std::string active_room;
    std::string active_outfit;
    std::vector<std::string> known_memory_tags;
};

struct DialogueResult {
    bool success = false;
    const DialogueNode* node = nullptr;
    std::vector<DialogueChoice> available_choices;
    std::string rejection_reason;
};

class DialogueEngine {
public:
    bool load_from_file(std::string_view path);
    void register_node(DialogueNode node);

    [[nodiscard]] DialogueResult evaluate_node(std::string_view node_id,
                                               const DialogueContext& context) const;
    [[nodiscard]] std::vector<const DialogueNode*> matching_lines(
        const DialogueContext& context, std::string_view category) const;
    [[nodiscard]] bool conditions_met(const DialogueConditions& conditions,
                                      const DialogueContext& context) const;
    DialogueResult apply_choice(const DialogueChoice& choice, CharacterSimState& character) const;

    [[nodiscard]] std::size_t node_count() const { return nodes_.size(); }

private:
    [[nodiscard]] bool time_matches(f32 hour, std::string_view bucket) const;

    std::unordered_map<std::string, DialogueNode> nodes_;
};

} // namespace eve::ai
