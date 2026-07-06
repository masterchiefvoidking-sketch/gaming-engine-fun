#include <eve/ai/relationship.hpp>
#include <eve/ai/schedule.hpp>
#include <eve/ai/social_ai.hpp>
#include <eve/ai/wardrobe.hpp>
#include <eve/world/simulation.hpp>
#include <gtest/gtest.h>

TEST(SocialAITest, PicksMoodDrivenActions) {
    eve::ai::RelationshipRegistry relationships;
    eve::ai::ScheduleLibrary schedules;
    eve::world::ApartmentSimulation simulation;

    eve::ai::SocialContext context;
    context.relationships = &relationships;
    context.schedules = &schedules;
    context.simulation = &simulation;
    context.current_hour = 20.0f;

    eve::ai::SocialAI social_ai(&context);

    eve::ai::CharacterState sad_state;
    sad_state.mood = eve::ai::Mood::Sad;
    sad_state.stress = 0.8f;

    const auto actions = social_ai.evaluate_character(1, sad_state);
    ASSERT_FALSE(actions.empty());
    const auto best = social_ai.pick_best_action(actions);
    EXPECT_EQ(best.intent, eve::ai::SocialIntent::Comfort);
}

TEST(SocialAITest, ReactsToSharedEvents) {
    eve::ai::RelationshipRegistry relationships;
    eve::ai::ScheduleLibrary schedules;
    eve::world::ApartmentSimulation simulation;

    eve::ai::SocialContext context;
    context.relationships = &relationships;
    context.schedules = &schedules;
    context.simulation = &simulation;
    context.current_hour = 12.0f;

    eve::ai::SocialAI social_ai(&context);
    social_ai.react_to_event(1, 2, "shared_meal");

    const eve::ai::RelationshipModel* rel = relationships.find(1, 2);
    ASSERT_NE(rel, nullptr);
    EXPECT_GT(rel->scores.affection, 0.5f);
}
