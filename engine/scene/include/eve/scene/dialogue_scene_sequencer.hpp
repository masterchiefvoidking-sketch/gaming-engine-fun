#pragma once

#include <eve/ai/dialogue_engine.hpp>
#include <eve/scene/scene_types.hpp>

#include <string>
#include <vector>

namespace eve::scene {

struct DialogueBeatResult {
    bool success = false;
    std::string line;
    std::string node_id;
    std::vector<ai::DialogueChoice> choices;
    std::string rejection_reason;
};

class DialogueSceneSequencer {
public:
    DialogueBeatResult play_beat(const SceneBeat& beat, ai::DialogueEngine& dialogue,
                                 const ai::DialogueContext& context) const;
    DialogueBeatResult play_node(std::string_view node_id, ai::DialogueEngine& dialogue,
                                   const ai::DialogueContext& context) const;
};

} // namespace eve::scene
