#pragma once

#include <eve/ai/relationship.hpp>
#include <eve/core/types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::ai {

enum class RomanceReactionType : u8 {
    Compliment = 0,
    Flirt,
    Gift,
    OutfitReaction,
    Comfort,
    SharedActivity,
    Rejection,
    Boundary,
};

struct RomanceReactionDefinition {
    std::string id;
    RomanceReactionType type = RomanceReactionType::Compliment;
    std::string dialogue_line;
    f32 trust_delta = 0.0f;
    f32 affection_delta = 0.0f;
    f32 mood_shift = 0.0f;
    std::vector<std::string> required_outfit_tags;
    f32 min_trust = 0.0f;
    std::string memory_tag;
};

struct RomanceContext {
    EntityId initiator = kInvalidEntity;
    EntityId target = kInvalidEntity;
    std::string room_id;
    std::string outfit_id;
    std::string mood = "neutral";
    f32 trust = 0.5f;
    f32 affection = 0.5f;
    bool consent_granted = true;
    std::vector<std::string> boundaries;
};

struct RomanceReactionResult {
    bool accepted = false;
    std::string dialogue;
    f32 trust_delta = 0.0f;
    f32 affection_delta = 0.0f;
    std::string memory_created;
    std::string reason;
};

class RomanceSimulationHooks {
public:
    bool load_reactions(std::string_view path);
    void register_reaction(RomanceReactionDefinition reaction);

    RomanceReactionResult trigger(std::string_view reaction_id, RomanceContext& context,
                                  RelationshipRegistry& relationships) const;

    [[nodiscard]] bool respects_boundaries(std::string_view reaction_id, const RomanceContext& context) const;

private:
    std::unordered_map<std::string, RomanceReactionDefinition> reactions_;
};

} // namespace eve::ai
