#pragma once

#include <eve/ai/relationship.hpp>
#include <eve/ai/relationship_stages.hpp>
#include <eve/ai/wardrobe.hpp>
#include <eve/core/types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::ai {

enum class ConsentState : u8 {
    Unknown = 0,
    Granted,
    Denied,
    Revoked,
};

enum class PrivacyLevel : u8 {
    Public = 0,
    SemiPrivate,
    Private,
    Intimate,
};

struct BoundaryRule {
    std::string id;
    std::string description;
    std::vector<std::string> blocked_actions;
    RelationshipStage min_stage = RelationshipStage::Stranger;
};

struct CharacterSimState {
    std::string character_id;
    Mood mood = Mood::Neutral;
    f32 energy = 1.0f;
    f32 comfort = 0.5f;
    f32 trust = 0.0f;
    f32 affection = 0.0f;
    f32 respect = 0.0f;
    f32 stress = 0.0f;
    f32 confidence = 0.5f;
    f32 shyness = 0.5f;
    f32 familiarity = 0.0f;
    std::string current_outfit_id;
    std::string current_room_id = "living_room";
    std::string current_activity;
    PrivacyLevel privacy_level = PrivacyLevel::SemiPrivate;
    ConsentState consent = ConsentState::Unknown;
    PersonalityTraits personality;
    CharacterPreferences preferences;
    std::vector<BoundaryRule> boundaries;
    std::vector<std::string> recent_interactions;
    RelationshipStage relationship_stage = RelationshipStage::Stranger;

    void record_interaction(std::string_view interaction_id);
    void apply_mood_delta(Mood new_mood, f32 intensity = 0.1f);
};

class CharacterProfileLibrary {
public:
    bool load_profile(std::string_view path);
    bool load_boundaries(std::string_view path);
    void register_state(CharacterSimState state);
    [[nodiscard]] CharacterSimState* find(std::string_view character_id);
    [[nodiscard]] const CharacterSimState* find(std::string_view character_id) const;

private:
    std::unordered_map<std::string, CharacterSimState> characters_;
};

} // namespace eve::ai
