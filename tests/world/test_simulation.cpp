#include <eve/world/simulation.hpp>
#include <gtest/gtest.h>

TEST(SimulationTest, AdvancesTimeAndWeather) {
    eve::world::ApartmentSimulation simulation;
    simulation.clock.hour = 6.0f;
    simulation.set_room_lights("living_room", true);
    simulation.set_weather(eve::world::WeatherType::Rain);

    simulation.update(60.0f);
    EXPECT_GT(simulation.clock.hour, 6.0f);
    EXPECT_EQ(simulation.clock.weather, eve::world::WeatherType::Rain);
    EXPECT_GT(simulation.room_light_level("living_room"), 0.0f);
}

TEST(SimulationTest, TimeOfDayBuckets) {
    eve::world::SimulationClock clock;
    clock.hour = 8.0f;
    EXPECT_EQ(clock.time_of_day(), eve::world::TimeOfDay::Morning);
    clock.hour = 19.0f;
    EXPECT_EQ(clock.time_of_day(), eve::world::TimeOfDay::Evening);
}
