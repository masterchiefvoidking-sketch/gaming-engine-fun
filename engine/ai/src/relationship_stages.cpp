#include <eve/ai/relationship_stages.hpp>

namespace eve::ai {

RelationshipStage compute_stage(const RelationshipProgressionScores& scores) {
    if (scores.trust >= 0.85f && scores.affection >= 0.85f && scores.familiarity >= 0.8f) {
        return RelationshipStage::Partner;
    }
    if (scores.affection >= 0.65f && scores.trust >= 0.6f && scores.familiarity >= 0.55f) {
        return RelationshipStage::RomanticInterest;
    }
    if (scores.trust >= 0.55f && scores.familiarity >= 0.5f) {
        return RelationshipStage::CloseFriend;
    }
    if (scores.trust >= 0.35f && scores.familiarity >= 0.25f) {
        return RelationshipStage::Friend;
    }
    if (scores.familiarity >= 0.1f || scores.trust >= 0.15f) {
        return RelationshipStage::Acquaintance;
    }
    return RelationshipStage::Stranger;
}

const char* to_string(RelationshipStage stage) {
    switch (stage) {
    case RelationshipStage::Acquaintance:
        return "acquaintance";
    case RelationshipStage::Friend:
        return "friend";
    case RelationshipStage::CloseFriend:
        return "close_friend";
    case RelationshipStage::RomanticInterest:
        return "romantic_interest";
    case RelationshipStage::Partner:
        return "partner";
    default:
        return "stranger";
    }
}

bool stage_allows_romantic_actions(RelationshipStage stage) {
    return static_cast<u8>(stage) >= static_cast<u8>(RelationshipStage::RomanticInterest);
}

u8 minimum_stage_level(RelationshipStage stage) {
    return static_cast<u8>(stage);
}

} // namespace eve::ai
