#pragma once

#include <eve/ai/relationship.hpp>
#include <eve/ai/schedule.hpp>
#include <eve/ai/wardrobe.hpp>
#include <eve/world/simulation.hpp>
#include <string>
#include <vector>

namespace eve::ai {

struct SocialAction {
    SocialIntent intent = SocialIntent::None;
    std::string label;
    std::string target_room;
    std::string interaction_id;
    EntityId initiator = kInvalidEntity;
    EntityId target = kInvalidEntity;
    f32 priority = 0.0f;
};

struct SocialContext {
    world::ApartmentSimulation* simulation = nullptr;
    RelationshipRegistry* relationships = nullptr;
    ScheduleLibrary* schedules = nullptr;
    f32 current_hour = 8.0f;
};

class SocialAI {
public:
    explicit SocialAI(SocialContext* context);

    [[nodiscard]] std::vector<SocialAction> evaluate_character(EntityId character,
                                                               const CharacterState& state) const;

    void react_to_event(EntityId character, EntityId other, const std::string& event_id);
    [[nodiscard]] SocialAction pick_best_action(const std::vector<SocialAction>& actions) const;

private:
    [[nodiscard]] std::vector<SocialAction> schedule_actions(EntityId character,
                                                             const CharacterState& state) const;
    [[nodiscard]] std::vector<SocialAction> relationship_actions(EntityId character,
                                                                 const CharacterState& state) const;
    [[nodiscard]] std::vector<SocialAction> mood_actions(EntityId character,
                                                         const CharacterState& state) const;

    SocialContext* context_ = nullptr;
};

} // namespace eve::ai
