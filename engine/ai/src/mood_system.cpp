#include <algorithm>
#include <eve/ai/mood_system.hpp>

namespace eve::ai {

void MoodSystem::apply_transition(CharacterSimState& character, const MoodTransition& transition) {
    character.mood = transition.to;
    character.trust = std::clamp(character.trust + transition.trust_delta, 0.0f, 1.0f);
    character.affection = std::clamp(character.affection + transition.affection_delta, 0.0f, 1.0f);
    character.comfort = std::clamp(character.comfort + transition.comfort_delta, 0.0f, 1.0f);
    character.stress = std::clamp(character.stress + transition.stress_delta, 0.0f, 1.0f);
}

void MoodSystem::apply_compliment(CharacterSimState& character, f32 intensity) {
    apply_transition(character, {character.mood, Mood::Happy, intensity * 0.05f, intensity * 0.08f,
                                 intensity * 0.1f, -intensity * 0.05f});
    character.confidence = std::min(character.confidence + intensity * 0.06f, 1.0f);
    character.shyness = std::max(character.shyness - intensity * 0.04f, 0.0f);
}

void MoodSystem::apply_rejection(CharacterSimState& character) {
    apply_transition(character,
                   {character.mood, Mood::Anxious, -0.02f, -0.01f, -0.08f, 0.1f});
    character.shyness = std::min(character.shyness + 0.05f, 1.0f);
}

void MoodSystem::apply_gift(CharacterSimState& character, bool liked) {
    if (liked) {
        apply_transition(character,
                       {character.mood, Mood::Happy, 0.05f, 0.1f, 0.12f, -0.05f});
    } else {
        apply_transition(character, {character.mood, Mood::Neutral, 0.01f, 0.02f, 0.0f, 0.02f});
    }
}

void MoodSystem::apply_shared_meal(CharacterSimState& character) {
    apply_transition(character,
                   {character.mood, Mood::Happy, 0.06f, 0.05f, 0.15f, -0.08f});
    character.energy = std::min(character.energy + 0.1f, 1.0f);
}

void MoodSystem::decay_mood(CharacterSimState& character, f32 delta_hours) {
    character.stress = std::max(0.0f, character.stress - delta_hours * 0.02f);
    if (character.mood == Mood::Happy && character.stress < 0.2f) {
        character.mood = Mood::Neutral;
    }
}

Mood MoodSystem::derive_mood(const CharacterSimState& character) const {
    if (character.stress > 0.7f) {
        return Mood::Stressed;
    }
    if (character.affection > 0.7f && character.comfort > 0.6f) {
        return Mood::Romantic;
    }
    if (character.energy < 0.3f) {
        return Mood::Tired;
    }
    return character.mood;
}

} // namespace eve::ai
