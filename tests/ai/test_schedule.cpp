#include <eve/ai/schedule.hpp>
#include <gtest/gtest.h>

TEST(ScheduleTest, FindsActiveEntry) {
    eve::ai::DailySchedule schedule;
    schedule.character_id = "alice";
    schedule.entries.push_back({7.0f, 9.0f, eve::ai::ScheduleActivity::Breakfast, "kitchen",
                                "brew_coffee", "Breakfast", 2});
    schedule.entries.push_back(
        {9.0f, 17.0f, eve::ai::ScheduleActivity::Work, "office", "work", "Work", 3});

    const eve::ai::ScheduleEntry* entry = schedule.active_entry(8.0f);
    ASSERT_NE(entry, nullptr);
    EXPECT_EQ(entry->room_id, "kitchen");
}
