#pragma once

#include <eve/ai/relationship.hpp>
#include <eve/core/types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::ai {

enum class MemoryType : u8 {
    FirstMeeting = 0,
    FavoriteGift,
    Compliment,
    RejectedInteraction,
    SharedMeal,
    MovieWatched,
    OutfitLiked,
    OutfitDisliked,
    ImportantDate,
    Promise,
    Apology,
    Boundary,
    RecurringHabit,
    Custom,
};

struct MemoryTemplate {
    std::string id;
    MemoryType type = MemoryType::Custom;
    std::string description_template;
    f32 default_emotional_weight = 0.5f;
    std::vector<std::string> dialogue_callbacks;
};

struct MemoryRecallResult {
    bool found = false;
    MemoryEntry entry;
    std::string callback_line;
};

class MemorySystem {
public:
    bool load_templates(std::string_view path);
    void register_template(MemoryTemplate tmpl);

    MemoryEntry create_memory(std::string_view template_id, std::string_view related_character,
                              u32 day_index, const std::string& detail = {}) const;
    MemoryRecallResult recall_by_tag(RelationshipModel& relationship,
                                     std::string_view tag) const;
    [[nodiscard]] std::vector<std::string> all_memory_tags(const RelationshipModel& relationship) const;
    [[nodiscard]] bool has_memory(const RelationshipModel& relationship,
                                  std::string_view tag) const;

private:
    std::unordered_map<std::string, MemoryTemplate> templates_;
};

} // namespace eve::ai
