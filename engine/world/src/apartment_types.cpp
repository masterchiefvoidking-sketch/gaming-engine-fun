#include <eve/world/apartment_types.hpp>

namespace eve::world {

const char* to_string(RoomType type) {
    switch (type) {
    case RoomType::Bedroom:
        return "bedroom";
    case RoomType::Bathroom:
        return "bathroom";
    case RoomType::LivingRoom:
        return "living_room";
    case RoomType::DiningRoom:
        return "dining_room";
    case RoomType::Kitchen:
        return "kitchen";
    case RoomType::Wardrobe:
        return "wardrobe";
    case RoomType::Office:
        return "office";
    case RoomType::Balcony:
        return "balcony";
    case RoomType::Hallway:
        return "hallway";
    case RoomType::LaundryRoom:
        return "laundry_room";
    case RoomType::Storage:
        return "storage";
    default:
        return "unknown";
    }
}

const char* to_string(InteractionCategory category) {
    switch (category) {
    case InteractionCategory::General:
        return "general";
    case InteractionCategory::Sleep:
        return "sleep";
    case InteractionCategory::Hygiene:
        return "hygiene";
    case InteractionCategory::Wardrobe:
        return "wardrobe";
    case InteractionCategory::Entertainment:
        return "entertainment";
    case InteractionCategory::Dining:
        return "dining";
    case InteractionCategory::Cleaning:
        return "cleaning";
    case InteractionCategory::Decorating:
        return "decorating";
    case InteractionCategory::Social:
        return "social";
    case InteractionCategory::Lighting:
        return "lighting";
    case InteractionCategory::Appliance:
        return "appliance";
    case InteractionCategory::Storage:
        return "storage";
    default:
        return "unknown";
    }
}

const char* to_string(ObjectState state) {
    switch (state) {
    case ObjectState::Idle:
        return "idle";
    case ObjectState::InUse:
        return "in_use";
    case ObjectState::Dirty:
        return "dirty";
    case ObjectState::Broken:
        return "broken";
    case ObjectState::Off:
        return "off";
    case ObjectState::On:
        return "on";
    case ObjectState::Open:
        return "open";
    case ObjectState::Closed:
        return "closed";
    default:
        return "unknown";
    }
}

const char* to_string(TimeOfDay time) {
    switch (time) {
    case TimeOfDay::Morning:
        return "morning";
    case TimeOfDay::Afternoon:
        return "afternoon";
    case TimeOfDay::Evening:
        return "evening";
    case TimeOfDay::Night:
        return "night";
    default:
        return "unknown";
    }
}

const char* to_string(WeatherType weather) {
    switch (weather) {
    case WeatherType::Clear:
        return "clear";
    case WeatherType::Cloudy:
        return "cloudy";
    case WeatherType::Rain:
        return "rain";
    case WeatherType::Storm:
        return "storm";
    case WeatherType::Snow:
        return "snow";
    default:
        return "unknown";
    }
}

RoomType room_type_from_string(std::string_view value) {
    if (value == "bedroom")
        return RoomType::Bedroom;
    if (value == "bathroom")
        return RoomType::Bathroom;
    if (value == "living_room")
        return RoomType::LivingRoom;
    if (value == "dining_room")
        return RoomType::DiningRoom;
    if (value == "kitchen")
        return RoomType::Kitchen;
    if (value == "wardrobe")
        return RoomType::Wardrobe;
    if (value == "office")
        return RoomType::Office;
    if (value == "balcony")
        return RoomType::Balcony;
    if (value == "hallway")
        return RoomType::Hallway;
    if (value == "laundry_room")
        return RoomType::LaundryRoom;
    if (value == "storage")
        return RoomType::Storage;
    return RoomType::Hallway;
}

InteractionCategory interaction_category_from_string(std::string_view value) {
    if (value == "sleep")
        return InteractionCategory::Sleep;
    if (value == "hygiene")
        return InteractionCategory::Hygiene;
    if (value == "wardrobe")
        return InteractionCategory::Wardrobe;
    if (value == "entertainment")
        return InteractionCategory::Entertainment;
    if (value == "dining")
        return InteractionCategory::Dining;
    if (value == "cleaning")
        return InteractionCategory::Cleaning;
    if (value == "decorating")
        return InteractionCategory::Decorating;
    if (value == "social")
        return InteractionCategory::Social;
    if (value == "lighting")
        return InteractionCategory::Lighting;
    if (value == "appliance")
        return InteractionCategory::Appliance;
    if (value == "storage")
        return InteractionCategory::Storage;
    return InteractionCategory::General;
}

} // namespace eve::world
