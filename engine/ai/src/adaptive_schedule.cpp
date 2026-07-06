#include <eve/ai/adaptive_schedule.hpp>

namespace eve::ai {

DailySchedule AdaptiveScheduleAI::adapt_schedule(const DailySchedule& base,
                                                 const ScheduleAdaptationContext& context) const {
    DailySchedule adapted = base;
    if (context.mood == Mood::Sad || context.mood == Mood::Stressed) {
        for (ScheduleEntry& entry : adapted.entries) {
            if (entry.activity == ScheduleActivity::Socialize) {
                entry.activity = ScheduleActivity::Relax;
                entry.room_id = "bedroom";
            }
        }
    }
    if (context.energy < 0.3f) {
        for (ScheduleEntry& entry : adapted.entries) {
            if (entry.activity == ScheduleActivity::Exercise) {
                entry.activity = ScheduleActivity::Relax;
            }
        }
    }
    if (context.weather == "rain" && context.stage >= RelationshipStage::Friend) {
        ScheduleEntry rainy;
        rainy.start_hour = 18.0f;
        rainy.end_hour = 20.0f;
        rainy.activity = ScheduleActivity::Leisure;
        rainy.room_id = "living_room";
        rainy.label = "Cozy rainy evening";
        rainy.priority = 2;
        adapted.entries.push_back(rainy);
    }
  if (context.player_invited && !context.invitation_room.empty()) {
        for (ScheduleEntry& entry : adapted.entries) {
            if (entry.start_hour <= 20.0f && entry.end_hour > 18.0f) {
                entry.room_id = context.invitation_room;
                entry.label = "Time with player";
                entry.priority = 3;
            }
        }
    }
    return adapted;
}

const ScheduleEntry* AdaptiveScheduleAI::resolve_activity(
    const DailySchedule& schedule, f32 hour, const ScheduleAdaptationContext& context) const {
    const DailySchedule adapted = adapt_schedule(schedule, context);
    return adapted.active_entry(hour);
}

void AdaptiveScheduleAI::accept_player_invitation(DailySchedule& schedule, f32 hour,
                                                  std::string_view room_id,
                                                  std::string_view activity_label) {
    ScheduleEntry invite;
    invite.start_hour = hour;
    invite.end_hour = hour + 2.0f;
    invite.activity = ScheduleActivity::Socialize;
    invite.room_id = std::string(room_id);
    invite.label = std::string(activity_label);
    invite.priority = 5;
    schedule.entries.push_back(invite);
}

} // namespace eve::ai
