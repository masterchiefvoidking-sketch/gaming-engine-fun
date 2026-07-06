#pragma once

#include <eve/ai/character_sim_state.hpp>
#include <eve/ai/memory_system.hpp>
#include <eve/ai/mood_system.hpp>
#include <eve/ai/relationship.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::ai {

enum class RomanceEventType : u8 {
    FirstCompliment = 0,
    FirstSharedMeal,
    FirstMovieNight,
    FirstGift,
    FirstDatePlanning,
    RainyDayConversation,
    LateNightTalk,
    MorningCoffee,
    OutfitReveal,
    Birthday,
    Apology,
    Comfort,
    Anniversary,
    Custom,
};

struct RomanceEventTemplate {
    std::string id;
    RomanceEventType type = RomanceEventType::Custom;
    std::string label;
    std::string dialogue_id;
    std::string memory_template_id;
    std::string required_room;
    f32 trust_reward = 0.05f;
    f32 affection_reward = 0.08f;
    bool one_time = true;
};

struct RomanceEventResult {
    bool triggered = false;
    bool already_completed = false;
    std::string dialogue_id;
    MemoryEntry memory_created;
    std::string message;
};

class RomanceEventSystem {
public:
    bool load_templates(std::string_view path);
    void register_template(RomanceEventTemplate tmpl);

    RomanceEventResult trigger(std::string_view event_id, CharacterSimState& character,
                               RelationshipModel& relationship, const MemorySystem& memory,
                               MoodSystem& mood);

    [[nodiscard]] bool was_completed(const RelationshipModel& relationship,
                                     std::string_view event_id) const;
    [[nodiscard]] std::size_t template_count() const { return templates_.size(); }

private:
    std::unordered_map<std::string, RomanceEventTemplate> templates_;
};

} // namespace eve::ai
