#include <eve/ai/relationship.hpp>
#include <gtest/gtest.h>

TEST(RelationshipTest, TracksScoresAndMemories) {
    eve::ai::RelationshipRegistry registry;
    eve::ai::RelationshipModel& rel = registry.get_or_create(1, 2);
    rel.adjust_scores(0.1f, 0.2f, 0.05f, 0.1f);
    rel.record_conversation("Hello there.");
    rel.add_shared_experience("Watched a movie");
    rel.add_memory({"movie_night", "Watched a movie together", "2", 0.7f, 1, {"entertainment"}});

    EXPECT_GT(rel.scores.affection, 0.5f);
    EXPECT_EQ(rel.conversation_history.size(), 1u);
    EXPECT_EQ(rel.shared_memories.size(), 1u);
    EXPECT_NE(registry.find(2, 1), nullptr);
}
