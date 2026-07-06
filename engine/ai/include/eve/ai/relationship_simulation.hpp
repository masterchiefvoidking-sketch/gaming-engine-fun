#pragma once

#include <eve/ai/adaptive_schedule.hpp>
#include <eve/ai/boundaries.hpp>
#include <eve/ai/character_sim_state.hpp>
#include <eve/ai/dialogue_engine.hpp>
#include <eve/ai/memory_system.hpp>
#include <eve/ai/mood_system.hpp>
#include <eve/ai/outfit_reactions.hpp>
#include <eve/ai/relationship.hpp>
#include <eve/ai/relationship_stages.hpp>
#include <eve/ai/romance_events.hpp>
#include <eve/ai/room_behavior.hpp>
#include <eve/ai/scene_interruption.hpp>
#include <eve/ai/schedule.hpp>

#include <string>

namespace eve::ai {

struct RelationshipSimState {
    EntityId player_id = 1;
    EntityId character_id = 2;
    f32 time_of_day = 18.0f;
    std::string weather = "rain";
    u32 day_index = 0;
};

struct InteractionResult {
    bool success = false;
    std::string dialogue_line;
    std::string rejection_reason;
    MemoryEntry memory_created;
    RelationshipStage new_stage = RelationshipStage::Stranger;
    Mood new_mood = Mood::Neutral;
};

class RelationshipSimulation {
public:
    bool initialize(std::string_view data_root);

    InteractionResult talk(std::string_view dialogue_node_id);
    InteractionResult offer_compliment();
    InteractionResult invite_to_dinner();
    InteractionResult trigger_outfit_dialogue();
    InteractionResult trigger_event(std::string_view event_id);
    InteractionResult make_choice(std::string_view node_id, std::string_view choice_id);

    bool save_state(std::string_view path) const;
    bool load_state(std::string_view path);

    [[nodiscard]] CharacterSimState* character();
    [[nodiscard]] const CharacterSimState* character() const;
    [[nodiscard]] const RelationshipModel* relationship() const;
    [[nodiscard]] RelationshipStage current_stage() const;
    [[nodiscard]] DialogueContext build_dialogue_context() const;

    CharacterProfileLibrary& profiles() { return profiles_; }
    DialogueEngine& dialogue() { return dialogue_; }
    MemorySystem& memories() { return memories_; }
    MoodSystem& mood() { return mood_; }

private:
    void sync_relationship_stage();
    InteractionResult execute_dialogue_node(std::string_view node_id);

    CharacterProfileLibrary profiles_;
    RelationshipRegistry relationships_;
    DialogueEngine dialogue_;
    MemorySystem memories_;
    MoodSystem mood_;
    BoundaryConsentSystem boundaries_;
    RoomBehaviorSystem room_behavior_;
    OutfitReactionSystem outfit_reactions_;
    RomanceEventSystem events_;
    AdaptiveScheduleAI schedule_ai_;
    ScheduleLibrary schedules_;
    SceneInterruptionHandler scene_handler_;
    RelationshipSimState sim_state_;
    std::string data_root_;
    bool initialized_ = false;
};

} // namespace eve::ai
