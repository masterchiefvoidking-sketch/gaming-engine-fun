#include <eve/ai/relationship.hpp>
#include <eve/ai/romance_hooks.hpp>
#include <gtest/gtest.h>

TEST(RomanceHooksTest, LoadsReactionsFromData) {
    eve::ai::RomanceSimulationHooks hooks;
    ASSERT_TRUE(hooks.load_reactions(EVE_DATA_DIR "/romance/reactions.json"));
}

TEST(RomanceHooksTest, TriggerUpdatesRelationship) {
    eve::ai::RomanceSimulationHooks hooks;
    hooks.load_reactions(EVE_DATA_DIR "/romance/reactions.json");
    eve::ai::RelationshipRegistry relationships;
    eve::ai::RomanceContext context;
    context.initiator = 1;
    context.target = 2;
    context.trust = 0.6f;
    context.consent_granted = true;

    const eve::ai::RomanceReactionResult result = hooks.trigger("compliment_outfit", context, relationships);
    EXPECT_TRUE(result.accepted);
    EXPECT_FALSE(result.dialogue.empty());
    EXPECT_GT(result.affection_delta, 0.0f);

    const eve::ai::RelationshipModel& rel = relationships.get_or_create(1, 2);
    EXPECT_GT(rel.scores.affection, 0.5f);
}

TEST(RomanceHooksTest, RejectsWithoutConsent) {
    eve::ai::RomanceSimulationHooks hooks;
    hooks.load_reactions(EVE_DATA_DIR "/romance/reactions.json");
    eve::ai::RelationshipRegistry relationships;
    eve::ai::RomanceContext context;
    context.consent_granted = false;

    const eve::ai::RomanceReactionResult result = hooks.trigger("flirt_compliment", context, relationships);
    EXPECT_FALSE(result.accepted);
}

TEST(RomanceHooksTest, RespectsBoundaries) {
    eve::ai::RomanceSimulationHooks hooks;
    eve::ai::RomanceContext context;
    context.boundaries.push_back("no_flirting");
    EXPECT_FALSE(hooks.respects_boundaries("flirt_compliment", context));
}
