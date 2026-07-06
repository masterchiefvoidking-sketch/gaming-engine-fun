#pragma once

#include <eve/core/types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::ai {

enum class ScheduleActivity : u8 {
    Sleep = 0,
    Wake,
    Hygiene,
    Breakfast,
    Work,
    Lunch,
    Relax,
    Socialize,
    Cook,
    Dinner,
    Exercise,
    Leisure,
    Custom,
};

struct ScheduleEntry {
    f32 start_hour = 0.0f;
    f32 end_hour = 0.0f;
    ScheduleActivity activity = ScheduleActivity::Custom;
    std::string room_id;
    std::string interaction_id;
    std::string label;
    u8 priority = 1;
};

struct DailySchedule {
    std::string character_id;
    std::vector<ScheduleEntry> entries;

    [[nodiscard]] const ScheduleEntry* active_entry(f32 hour) const;
    [[nodiscard]] std::vector<ScheduleEntry> entries_for_room(std::string_view room_id) const;
};

class ScheduleLibrary {
public:
    void add_schedule(DailySchedule schedule);
    [[nodiscard]] const DailySchedule* find(std::string_view character_id) const;

private:
    std::unordered_map<std::string, DailySchedule> schedules_;
};

} // namespace eve::ai
