#include <eve/polish/dialogue_polish.hpp>

namespace eve::polish {

DialoguePolishResult DialoguePolish::compliment(ai::RelationshipSimulation& sim) {
    DialoguePolishResult result;
    const ai::InteractionResult interaction = sim.offer_compliment();
    result.success = interaction.success;
    result.line = interaction.dialogue_line;
    result.rejection_reason = interaction.rejection_reason;
    result.reaction_type = interaction.success ? "compliment_warm" : "boundary";
    return result;
}

DialoguePolishResult DialoguePolish::apology(ai::RelationshipSimulation& sim) {
    DialoguePolishResult result;
    const ai::InteractionResult interaction = sim.talk("apology_soft");
    result.success = interaction.success;
    result.line = interaction.dialogue_line;
    result.reaction_type = "comfort";
    return result;
}

DialoguePolishResult DialoguePolish::outfit_comment(ai::RelationshipSimulation& sim) {
    DialoguePolishResult result;
    const ai::InteractionResult interaction = sim.trigger_outfit_dialogue();
    result.success = interaction.success;
    result.line = interaction.dialogue_line;
    result.rejection_reason = interaction.rejection_reason;
    result.reaction_type = "outfit_reaction";
    return result;
}

DialoguePolishResult DialoguePolish::room_comment(ai::RelationshipSimulation& sim,
                                                  std::string_view room_id) {
    DialoguePolishResult result;
    const std::string node = "room_" + std::string(room_id) + "_talk";
    const ai::InteractionResult interaction = sim.talk(node);
    if (!interaction.success) {
        const ai::InteractionResult fallback = sim.talk("room_" + std::string(room_id) + "_1");
        result.success = fallback.success;
        result.line = fallback.dialogue_line;
    } else {
        result.success = true;
        result.line = interaction.dialogue_line;
    }
    result.reaction_type = "room_ambient";
    return result;
}

DialoguePolishResult DialoguePolish::memory_callback(ai::RelationshipSimulation& sim) {
    DialoguePolishResult result;
    const ai::InteractionResult interaction = sim.talk("memory_callback_1");
    result.success = interaction.success;
    result.line = interaction.dialogue_line;
    result.reaction_type = "memory";
    return result;
}

DialoguePolishResult DialoguePolish::mood_reaction(ai::RelationshipSimulation& sim,
                                                   std::string_view mood) {
    DialoguePolishResult result;
    const std::string node = "mood_" + std::string(mood) + "_1";
    const ai::InteractionResult interaction = sim.talk(node);
    result.success = interaction.success;
    result.line = interaction.dialogue_line;
    result.reaction_type = std::string(mood);
    return result;
}

DialoguePolishResult DialoguePolish::boundary_response(ai::RelationshipSimulation& sim,
                                                        std::string_view action_id) {
    DialoguePolishResult result;
    (void)action_id;
    const ai::InteractionResult interaction = sim.talk("compliment_blush");
    result.success = interaction.success;
    result.line = interaction.rejection_reason.empty() ? interaction.dialogue_line
                                                       : interaction.rejection_reason;
    result.reaction_type = "boundary";
    return result;
}

} // namespace eve::polish
