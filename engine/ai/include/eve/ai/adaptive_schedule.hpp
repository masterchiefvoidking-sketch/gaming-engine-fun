#pragma once

#include <eve/ai/character_sim_state.hpp>
#include <eve/ai/schedule.hpp>
#include <eve/ai/wardrobe.hpp>

namespace eve::ai {

struct ScheduleAdaptationContext {
    Mood mood = Mood::Neutral;
    RelationshipStage stage = RelationshipStage::Stranger;
    f32 energy = 1.0f;
    std::string weather = "clear";
    u8 day_of_week = 0;
    bool player_invited = false;
    std::string invitation_room;
};

class AdaptiveScheduleAI {
public:
    [[nodiscard]] DailySchedule adapt_schedule(const DailySchedule& base,
                                               const ScheduleAdaptationContext& context) const;
    [[nodiscard]] const ScheduleEntry* resolve_activity(const DailySchedule& schedule, f32 hour,
                                                        const ScheduleAdaptationContext& context) const;
    void accept_player_invitation(DailySchedule& schedule, f32 hour,
                                  std::string_view room_id, std::string_view activity_label);
};

} // namespace eve::ai
