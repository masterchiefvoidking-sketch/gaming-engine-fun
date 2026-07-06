#pragma once

#include <eve/ai/wardrobe.hpp>
#include <eve/core/types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::ai {

struct PersonalityTraits {
    f32 openness = 0.5f;
    f32 conscientiousness = 0.5f;
    f32 extraversion = 0.5f;
    f32 agreeableness = 0.5f;
    f32 neuroticism = 0.5f;
    f32 humor = 0.5f;
};

struct CharacterPreferences {
    std::vector<std::string> favorite_rooms;
    std::vector<std::string> favorite_activities;
    std::vector<std::string> favorite_foods;
    std::vector<std::string> disliked_activities;
};

struct CharacterState {
    Mood mood = Mood::Neutral;
    f32 stress = 0.0f;
    f32 energy = 1.0f;
    f32 hunger = 0.0f;
    PersonalityTraits personality;
    CharacterPreferences preferences;
    WardrobeProfile wardrobe;
    std::vector<std::string> personal_goals;
};

struct MemoryEntry {
    std::string id;
    std::string description;
    std::string related_character;
    f32 emotional_weight = 0.0f;
    u32 day_index = 0;
    std::vector<std::string> tags;
};

struct RelationshipScores {
    f32 trust = 0.5f;
    f32 affection = 0.5f;
    f32 respect = 0.5f;
    f32 humor = 0.5f;
};

struct RelationshipModel {
    EntityId character_a = kInvalidEntity;
    EntityId character_b = kInvalidEntity;
    RelationshipScores scores;
    std::vector<MemoryEntry> shared_memories;
    std::vector<std::string> conversation_history;
    std::vector<std::string> shared_experiences;

    void record_conversation(std::string_view line);
    void add_shared_experience(std::string_view experience);
    void add_memory(MemoryEntry memory);
    void adjust_scores(f32 trust_delta, f32 affection_delta, f32 respect_delta, f32 humor_delta);
};

class RelationshipRegistry {
public:
    RelationshipModel& get_or_create(EntityId a, EntityId b);
    [[nodiscard]] RelationshipModel* find(EntityId a, EntityId b);
    [[nodiscard]] const RelationshipModel* find(EntityId a, EntityId b) const;
    [[nodiscard]] std::size_t count() const { return relationships_.size(); }

private:
    static std::string make_key(EntityId a, EntityId b);

    std::unordered_map<std::string, RelationshipModel> relationships_;
};

} // namespace eve::ai
