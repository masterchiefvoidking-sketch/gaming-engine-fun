#pragma once

#include <eve/ai/relationship_simulation.hpp>

#include <string>

namespace eve::polish {

struct DialoguePolishResult {
    bool success = false;
    std::string line;
    std::string reaction_type;
    std::string rejection_reason;
};

class DialoguePolish {
public:
    DialoguePolishResult compliment(ai::RelationshipSimulation& sim);
    DialoguePolishResult apology(ai::RelationshipSimulation& sim);
    DialoguePolishResult outfit_comment(ai::RelationshipSimulation& sim);
    DialoguePolishResult room_comment(ai::RelationshipSimulation& sim, std::string_view room_id);
    DialoguePolishResult memory_callback(ai::RelationshipSimulation& sim);
    DialoguePolishResult mood_reaction(ai::RelationshipSimulation& sim, std::string_view mood);
    DialoguePolishResult boundary_response(ai::RelationshipSimulation& sim,
                                           std::string_view action_id);
};

} // namespace eve::polish
