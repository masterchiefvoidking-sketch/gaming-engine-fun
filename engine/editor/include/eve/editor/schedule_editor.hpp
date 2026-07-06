#pragma once

#include <eve/ai/schedule.hpp>
#include <string>

namespace eve::editor {

class ScheduleEditor {
public:
    void load_schedule(const ai::DailySchedule& schedule);
    bool add_entry(const ai::ScheduleEntry& entry);
    [[nodiscard]] const ai::DailySchedule& schedule() const { return schedule_; }

private:
    ai::DailySchedule schedule_;
};

} // namespace eve::editor
