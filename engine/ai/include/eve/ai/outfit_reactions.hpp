#pragma once

#include <eve/ai/character_sim_state.hpp>
#include <eve/ai/relationship_stages.hpp>

#include <string>
#include <vector>

namespace eve::ai {

struct OutfitReaction {
    std::string id;
    std::string outfit_id;
    std::vector<std::string> outfit_tags;
    std::string dialogue_line;
    std::string memory_tag;
    f32 affection_delta = 0.0f;
    f32 confidence_delta = 0.0f;
    RelationshipStage min_stage = RelationshipStage::Acquaintance;
};

class OutfitReactionSystem {
public:
    bool load_from_file(std::string_view path);
    void register_reaction(OutfitReaction reaction);

    [[nodiscard]] const OutfitReaction* find_reaction(std::string_view outfit_id,
                                                      const std::vector<std::string>& outfit_tags,
                                                      RelationshipStage stage) const;
    bool apply_reaction(const OutfitReaction& reaction, CharacterSimState& character) const;

    [[nodiscard]] std::size_t reaction_count() const { return reactions_.size(); }

private:
    std::vector<OutfitReaction> reactions_;
};

} // namespace eve::ai
