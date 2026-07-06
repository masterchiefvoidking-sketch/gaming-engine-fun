#include <eve/ai/adaptive_schedule.hpp>
#include <eve/ai/boundaries.hpp>
#include <eve/ai/dialogue_engine.hpp>
#include <eve/ai/memory_system.hpp>
#include <eve/ai/mood_system.hpp>
#include <eve/ai/outfit_reactions.hpp>
#include <eve/ai/relationship_simulation.hpp>
#include <eve/ai/relationship_stages.hpp>
#include <eve/ai/room_behavior.hpp>
#include <eve/ai/romance_events.hpp>
#include <eve/rainy_demo/rainy_evening_demo.hpp>

#include <gtest/gtest.h>

#include <filesystem>

namespace {

constexpr const char* kDataRoot = EVE_DATA_DIR;
constexpr const char* kRomancePath = EVE_DATA_DIR "/romance_sim";

} // namespace

TEST(RelationshipStageTest, ProgressesThroughStages) {
    eve::ai::RelationshipProgressionScores scores;
    scores.familiarity = 0.12f;
    EXPECT_EQ(eve::ai::compute_stage(scores), eve::ai::RelationshipStage::Acquaintance);

    scores.trust = 0.4f;
    scores.familiarity = 0.3f;
    EXPECT_EQ(eve::ai::compute_stage(scores), eve::ai::RelationshipStage::Friend);

    scores.trust = 0.7f;
    scores.affection = 0.7f;
    scores.familiarity = 0.6f;
    EXPECT_EQ(eve::ai::compute_stage(scores), eve::ai::RelationshipStage::RomanticInterest);

    scores.trust = 0.9f;
    scores.affection = 0.9f;
    scores.familiarity = 0.85f;
    EXPECT_EQ(eve::ai::compute_stage(scores), eve::ai::RelationshipStage::Partner);
}

TEST(BoundaryConsentTest, BlocksListedActions) {
    eve::ai::CharacterSimState character;
    character.consent = eve::ai::ConsentState::Granted;
    eve::ai::BoundaryRule rule;
    rule.id = "test";
    rule.description = "No hugs yet";
    rule.blocked_actions = {"hug"};
    character.boundaries.push_back(rule);

    eve::ai::BoundaryConsentSystem boundaries;
    const eve::ai::ConsentCheckResult hug =
        boundaries.check_action("hug", character, eve::ai::RelationshipStage::Partner);
    EXPECT_FALSE(hug.allowed);
    EXPECT_FALSE(hug.rejection_line.empty());

    const eve::ai::ConsentCheckResult compliment =
        boundaries.check_action("compliment", character, eve::ai::RelationshipStage::Stranger);
    EXPECT_TRUE(compliment.allowed);
}

TEST(BoundaryConsentTest, DeniesWhenConsentRevoked) {
    eve::ai::CharacterSimState character;
    character.consent = eve::ai::ConsentState::Revoked;

    eve::ai::BoundaryConsentSystem boundaries;
    const eve::ai::ConsentCheckResult result =
        boundaries.check_action("compliment", character, eve::ai::RelationshipStage::Friend);
    EXPECT_FALSE(result.allowed);
}

TEST(DialogueEngineTest, EvaluatesConditions) {
    eve::ai::DialogueEngine engine;
    ASSERT_TRUE(engine.load_from_file(std::string(kRomancePath) + "/dialogue.json"));
    EXPECT_GE(engine.node_count(), 50u);

    eve::ai::CharacterSimState character;
    character.trust = 0.5f;
    character.affection = 0.5f;
    character.current_room_id = "living_room";

    eve::ai::DialogueContext ctx;
    ctx.character = &character;
    ctx.stage = eve::ai::RelationshipStage::Friend;
    ctx.time_of_day = 18.0f;
    ctx.active_room = "living_room";

    const eve::ai::DialogueResult evening =
        engine.evaluate_node("greet_evening", ctx);
    EXPECT_TRUE(evening.success);
    EXPECT_NE(evening.node, nullptr);

    ctx.time_of_day = 8.0f;
    const eve::ai::DialogueResult wrong_time =
        engine.evaluate_node("greet_evening", ctx);
    EXPECT_FALSE(wrong_time.success);
}

TEST(MemorySystemTest, CreatesAndRecallsMemories) {
    eve::ai::MemorySystem memories;
    ASSERT_TRUE(memories.load_templates(std::string(kRomancePath) + "/memory_templates.json"));

    eve::ai::RelationshipModel relationship;
    const eve::ai::MemoryEntry entry =
        memories.create_memory("compliment_received", "player", 1, "Nice smile");
    relationship.add_memory(entry);

    EXPECT_TRUE(memories.has_memory(relationship, "compliment_received"));
    const eve::ai::MemoryRecallResult recall =
        memories.recall_by_tag(relationship, "compliment_received");
    EXPECT_TRUE(recall.found);
    EXPECT_FALSE(recall.callback_line.empty());
}

TEST(MoodSystemTest, ComplimentRaisesAffection) {
    eve::ai::CharacterSimState character;
    character.affection = 0.3f;
    character.trust = 0.3f;

    eve::ai::MoodSystem mood;
    mood.apply_compliment(character, 0.2f);

    EXPECT_GT(character.affection, 0.3f);
    EXPECT_GT(character.trust, 0.3f);
    EXPECT_EQ(mood.derive_mood(character), eve::ai::Mood::Happy);
}

TEST(OutfitReactionTest, SelectsOutfitReaction) {
    eve::ai::OutfitReactionSystem reactions;
    ASSERT_TRUE(reactions.load_from_file(std::string(kRomancePath) + "/outfit_reactions.json"));
    EXPECT_GE(reactions.reaction_count(), 15u);

    const eve::ai::OutfitReaction* reaction = reactions.find_reaction(
        "cozy_sweater", {"casual", "cozy"}, eve::ai::RelationshipStage::Friend);
    ASSERT_NE(reaction, nullptr);
    EXPECT_FALSE(reaction->dialogue_line.empty());
}

TEST(RoomBehaviorTest, FiltersByRoom) {
    eve::ai::RoomBehaviorSystem rooms;
    ASSERT_TRUE(rooms.load_from_file(std::string(kRomancePath) + "/room_reactions.json"));

    eve::ai::CharacterSimState character;
    const auto living = rooms.actions_for_room("living_room", character,
                                               eve::ai::RelationshipStage::Friend);
    EXPECT_GE(living.size(), 1u);

    const auto bathroom = rooms.actions_for_room("bathroom", character,
                                                 eve::ai::RelationshipStage::Stranger);
    EXPECT_GE(bathroom.size(), 1u);
}

TEST(AdaptiveScheduleTest, AdaptsForRainAndInvitation) {
    eve::ai::DailySchedule schedule;
    schedule.character_id = "aiko";
    eve::ai::ScheduleEntry work;
    work.start_hour = 9.0f;
    work.end_hour = 17.0f;
    work.activity = eve::ai::ScheduleActivity::Work;
    schedule.entries.push_back(work);

    eve::ai::ScheduleAdaptationContext ctx;
    ctx.weather = "rain";
    ctx.stage = eve::ai::RelationshipStage::Friend;
    ctx.player_invited = true;
    ctx.invitation_room = "living_room";

    eve::ai::AdaptiveScheduleAI ai;
    const eve::ai::DailySchedule adapted = ai.adapt_schedule(schedule, ctx);
    EXPECT_GT(adapted.entries.size(), schedule.entries.size());
}

TEST(RomanceEventTest, TriggersOneTimeEvent) {
    eve::ai::RomanceEventSystem events;
    ASSERT_TRUE(events.load_templates(std::string(kRomancePath) + "/event_templates.json"));

    eve::ai::CharacterSimState character;
    character.current_room_id = "living_room";
    eve::ai::RelationshipModel relationship;
    eve::ai::MemorySystem memories;
    memories.load_templates(std::string(kRomancePath) + "/memory_templates.json");
    eve::ai::MoodSystem mood;

    const eve::ai::RomanceEventResult first =
        events.trigger("rainy_day_conversation", character, relationship, memories, mood);
    EXPECT_TRUE(first.triggered);

    const eve::ai::RomanceEventResult second =
        events.trigger("rainy_day_conversation", character, relationship, memories, mood);
    EXPECT_TRUE(second.already_completed);
}

TEST(RelationshipSimulationTest, SaveLoadPersistence) {
    const std::filesystem::path save_path =
        std::filesystem::temp_directory_path() / "eve_romance_sim_test.json";

    eve::ai::RelationshipSimulation sim;
    ASSERT_TRUE(sim.initialize(kDataRoot));

    sim.offer_compliment();
    sim.invite_to_dinner();
    ASSERT_TRUE(sim.save_state(save_path.string()));

    eve::ai::RelationshipSimulation reloaded;
    ASSERT_TRUE(reloaded.initialize(kDataRoot));
    ASSERT_TRUE(reloaded.load_state(save_path.string()));

    const eve::ai::CharacterSimState* original = sim.character();
    const eve::ai::CharacterSimState* loaded = reloaded.character();
    ASSERT_NE(original, nullptr);
    ASSERT_NE(loaded, nullptr);
    EXPECT_NEAR(loaded->trust, original->trust, 0.001f);
    EXPECT_NEAR(loaded->affection, original->affection, 0.001f);

    const eve::ai::RelationshipModel* original_rel = sim.relationship();
    const eve::ai::RelationshipModel* loaded_rel = reloaded.relationship();
    ASSERT_NE(original_rel, nullptr);
    ASSERT_NE(loaded_rel, nullptr);
    EXPECT_EQ(loaded_rel->shared_memories.size(), original_rel->shared_memories.size());

    std::filesystem::remove(save_path);
}

TEST(RainyEveningDemoTest, RunsVerticalSliceFlow) {
    eve::rainy_demo::RainyEveningDemo demo;
    ASSERT_TRUE(demo.initialize(kDataRoot));

    const auto talk = demo.talk_in_living_room();
    EXPECT_TRUE(talk.success);
    EXPECT_FALSE(talk.dialogue.empty());

    const auto compliment = demo.offer_compliment();
    EXPECT_TRUE(compliment.success);

    const auto dinner = demo.invite_to_dinner();
    EXPECT_TRUE(dinner.success);

    const auto outfit = demo.trigger_outfit_dialogue();
    EXPECT_TRUE(outfit.success);

    const auto blocked = demo.try_blocked_action("hug");
    EXPECT_FALSE(blocked.success);

    const std::filesystem::path save_path =
        std::filesystem::temp_directory_path() / "eve_rainy_demo_test.json";
    EXPECT_TRUE(demo.save_state(save_path.string()));

    eve::rainy_demo::RainyEveningDemo reloaded;
    ASSERT_TRUE(reloaded.initialize(kDataRoot));
    EXPECT_TRUE(reloaded.load_state(save_path.string()));

    const auto recall = reloaded.recall_memory("compliment_received");
    EXPECT_TRUE(recall.success);

    EXPECT_GE(demo.history().size(), 5u);
    std::filesystem::remove(save_path);
    demo.shutdown();
}
