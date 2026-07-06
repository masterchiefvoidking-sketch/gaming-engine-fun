#pragma once

#include <eve/ai/character_sim_state.hpp>
#include <eve/ai/wardrobe.hpp>

namespace eve::ai {

struct MoodTransition {
    Mood from = Mood::Neutral;
    Mood to = Mood::Neutral;
    f32 trust_delta = 0.0f;
    f32 affection_delta = 0.0f;
    f32 comfort_delta = 0.0f;
    f32 stress_delta = 0.0f;
};

class MoodSystem {
public:
    void apply_transition(CharacterSimState& character, const MoodTransition& transition);
    void apply_compliment(CharacterSimState& character, f32 intensity = 0.1f);
    void apply_rejection(CharacterSimState& character);
    void apply_gift(CharacterSimState& character, bool liked);
    void apply_shared_meal(CharacterSimState& character);
    void decay_mood(CharacterSimState& character, f32 delta_hours);

    [[nodiscard]] Mood derive_mood(const CharacterSimState& character) const;
};

} // namespace eve::ai
