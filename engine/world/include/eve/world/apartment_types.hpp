#pragma once

#include <eve/core/types.hpp>
#include <string>
#include <string_view>

namespace eve::world {

enum class RoomType : u8 {
    Bedroom = 0,
    Bathroom,
    LivingRoom,
    DiningRoom,
    Kitchen,
    Wardrobe,
    Office,
    Balcony,
    Hallway,
    LaundryRoom,
    Storage,
    Count
};

enum class InteractionCategory : u8 {
    General = 0,
    Sleep,
    Hygiene,
    Wardrobe,
    Entertainment,
    Dining,
    Cleaning,
    Decorating,
    Social,
    Lighting,
    Appliance,
    Storage,
    Count
};

enum class ObjectState : u8 {
    Idle = 0,
    InUse,
    Dirty,
    Broken,
    Off,
    On,
    Open,
    Closed,
};

enum class TimeOfDay : u8 {
    Morning = 0,
    Afternoon,
    Evening,
    Night,
};

enum class WeatherType : u8 {
    Clear = 0,
    Cloudy,
    Rain,
    Storm,
    Snow,
};

const char* to_string(RoomType type);
const char* to_string(InteractionCategory category);
const char* to_string(ObjectState state);
const char* to_string(TimeOfDay time);
const char* to_string(WeatherType weather);

RoomType room_type_from_string(std::string_view value);
InteractionCategory interaction_category_from_string(std::string_view value);

} // namespace eve::world
