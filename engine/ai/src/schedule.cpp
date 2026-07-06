#include <eve/ai/schedule.hpp>

namespace eve::ai {

const ScheduleEntry* DailySchedule::active_entry(f32 hour) const {
    const ScheduleEntry* best = nullptr;
    for (const ScheduleEntry& entry : entries) {
        if (hour >= entry.start_hour && hour < entry.end_hour) {
            if (best == nullptr || entry.priority > best->priority) {
                best = &entry;
            }
        }
    }
    return best;
}

std::vector<ScheduleEntry> DailySchedule::entries_for_room(std::string_view room_id) const {
    std::vector<ScheduleEntry> result;
    for (const ScheduleEntry& entry : entries) {
        if (entry.room_id == room_id) {
            result.push_back(entry);
        }
    }
    return result;
}

void ScheduleLibrary::add_schedule(DailySchedule schedule) {
    schedules_[schedule.character_id] = std::move(schedule);
}

const DailySchedule* ScheduleLibrary::find(std::string_view character_id) const {
    const auto it = schedules_.find(std::string(character_id));
    if (it == schedules_.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace eve::ai
