#include <eve/scene/dialogue_scene_sequencer.hpp>

namespace eve::scene {

DialogueBeatResult DialogueSceneSequencer::play_beat(const SceneBeat& beat,
                                                     ai::DialogueEngine& dialogue,
                                                     const ai::DialogueContext& context) const {
    if (beat.dialogue_node_id.empty()) {
        DialogueBeatResult result;
        result.success = true;
        return result;
    }
    return play_node(beat.dialogue_node_id, dialogue, context);
}

DialogueBeatResult DialogueSceneSequencer::play_node(std::string_view node_id,
                                                     ai::DialogueEngine& dialogue,
                                                     const ai::DialogueContext& context) const {
    DialogueBeatResult result;
    const ai::DialogueResult node = dialogue.evaluate_node(node_id, context);
    if (!node.success || node.node == nullptr) {
        result.rejection_reason = node.rejection_reason;
        return result;
    }
    result.success = true;
    result.line = node.node->line;
    result.node_id = node.node->id;
    result.choices = node.available_choices;
    return result;
}

} // namespace eve::scene
