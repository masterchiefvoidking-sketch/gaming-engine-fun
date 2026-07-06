#include <eve/editor/romance_sim_tools.hpp>
#include <eve/ai/relationship_simulation.hpp>

#include <gtest/gtest.h>

TEST(RomanceSimEditorToolsTest, PanelsExposeLoadedData) {
    eve::ai::RelationshipSimulation simulation;
    ASSERT_TRUE(simulation.initialize(EVE_DATA_DIR));

    eve::editor::DialogueGraphEditorPanel dialogue_panel;
    dialogue_panel.set_engine(simulation.dialogue());
    EXPECT_GE(dialogue_panel.node_count(), 50u);

    eve::editor::MemoryDebuggerPanel memory_panel;
    const eve::ai::RelationshipModel* rel = simulation.relationship();
    ASSERT_NE(rel, nullptr);
    memory_panel.set_system(simulation.memories(), *rel);

    eve::editor::RelationshipInspectorPanel relationship_panel;
    relationship_panel.set_simulation(simulation);
    EXPECT_FALSE(relationship_panel.summary().empty());

    const eve::ai::CharacterSimState* character = simulation.character();
    ASSERT_NE(character, nullptr);

    eve::editor::MoodInspectorPanel mood_panel;
    mood_panel.set_character(*character);

    eve::editor::BoundaryConsentEditorPanel boundary_panel;
    boundary_panel.set_character(*character);
    EXPECT_GE(boundary_panel.boundary_count(), 1u);
    const eve::ai::ConsentCheckResult hug =
        boundary_panel.preview_action("hug", simulation.current_stage());
    EXPECT_FALSE(hug.allowed);

    eve::editor::OutfitReactionEditorPanel outfit_panel;
    eve::ai::OutfitReactionSystem outfit_system;
    outfit_system.load_from_file(EVE_DATA_DIR "/romance_sim/outfit_reactions.json");
    outfit_panel.set_system(outfit_system);
    EXPECT_GE(outfit_panel.reaction_count(), 15u);

    eve::editor::RoomBehaviorEditorPanel room_panel;
    eve::ai::RoomBehaviorSystem room_system;
    room_system.load_from_file(EVE_DATA_DIR "/romance_sim/room_reactions.json");
    room_panel.set_system(room_system);
    EXPECT_GE(room_panel.action_count(), 10u);
    EXPECT_GE(room_panel.actions_for_room("living_room"), 1u);
}
