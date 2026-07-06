#pragma once

#include <eve/ai/character_sim_state.hpp>
#include <eve/ai/relationship_stages.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::ai {

struct RoomBehaviorAction {
    std::string id;
    std::string room_id;
    std::string label;
    std::string dialogue_id;
    std::string interaction_id;
    RelationshipStage min_stage = RelationshipStage::Stranger;
    PrivacyLevel required_privacy = PrivacyLevel::Public;
};

class RoomBehaviorSystem {
public:
    bool load_from_file(std::string_view path);
    void register_action(RoomBehaviorAction action);

    [[nodiscard]] std::vector<RoomBehaviorAction> actions_for_room(
        std::string_view room_id, const CharacterSimState& character,
        RelationshipStage stage) const;
    [[nodiscard]] const RoomBehaviorAction* pick_best(std::string_view room_id,
                                                      const CharacterSimState& character,
                                                      RelationshipStage stage) const;

    [[nodiscard]] std::size_t action_count() const { return actions_.size(); }

private:
    std::vector<RoomBehaviorAction> actions_;
};

} // namespace eve::ai
