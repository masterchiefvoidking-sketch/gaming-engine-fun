#pragma once

#include <eve/core/types.hpp>
#include <string>

namespace eve::ai {

enum class RelationshipStage : u8 {
    Stranger = 0,
    Acquaintance,
    Friend,
    CloseFriend,
    RomanticInterest,
    Partner,
    Count
};

struct RelationshipProgressionScores {
    f32 trust = 0.0f;
    f32 affection = 0.0f;
    f32 respect = 0.0f;
    f32 familiarity = 0.0f;
};

[[nodiscard]] RelationshipStage compute_stage(const RelationshipProgressionScores& scores);
[[nodiscard]] const char* to_string(RelationshipStage stage);
[[nodiscard]] bool stage_allows_romantic_actions(RelationshipStage stage);
[[nodiscard]] u8 minimum_stage_level(RelationshipStage stage);

} // namespace eve::ai
