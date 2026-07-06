#include <eve/editor/schedule_editor.hpp>

namespace eve::editor {

void ScheduleEditor::load_schedule(const ai::DailySchedule& schedule) {
    schedule_ = schedule;
}

bool ScheduleEditor::add_entry(const ai::ScheduleEntry& entry) {
    schedule_.entries.push_back(entry);
    return true;
}

} // namespace eve::editor
