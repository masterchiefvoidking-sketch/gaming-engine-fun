#pragma once

#include <eve/ai/adaptive_schedule.hpp>
#include <eve/ai/boundaries.hpp>
#include <eve/ai/dialogue_engine.hpp>
#include <eve/ai/memory_system.hpp>
#include <eve/ai/mood_system.hpp>
#include <eve/ai/outfit_reactions.hpp>
#include <eve/ai/relationship_simulation.hpp>
#include <eve/ai/romance_events.hpp>
#include <eve/ai/room_behavior.hpp>

#include <string>
#include <vector>

namespace eve::editor {

class DialogueGraphEditorPanel {
public:
    void set_engine(const ai::DialogueEngine& engine);
    [[nodiscard]] std::size_t node_count() const { return node_count_; }
    [[nodiscard]] const std::string& selected_node_id() const { return selected_node_id_; }
    void select_node(std::string_view node_id) { selected_node_id_ = std::string(node_id); }

private:
    std::size_t node_count_ = 0;
    std::string selected_node_id_;
};

class MemoryDebuggerPanel {
public:
    void set_system(const ai::MemorySystem& memories, const ai::RelationshipModel& relationship);
    [[nodiscard]] std::size_t template_count() const { return template_count_; }
    [[nodiscard]] std::size_t active_memory_count() const { return active_memory_count_; }
    [[nodiscard]] const std::string& last_recall_line() const { return last_recall_line_; }
    void recall(std::string_view tag);

private:
    const ai::MemorySystem* memories_ = nullptr;
    const ai::RelationshipModel* relationship_ = nullptr;
    std::size_t template_count_ = 0;
    std::size_t active_memory_count_ = 0;
    std::string last_recall_line_;
};

class RelationshipInspectorPanel {
public:
    void set_simulation(const ai::RelationshipSimulation& simulation);
    [[nodiscard]] ai::RelationshipStage stage() const { return stage_; }
    [[nodiscard]] std::string summary() const;

private:
    ai::RelationshipStage stage_ = ai::RelationshipStage::Stranger;
    const ai::RelationshipModel* relationship_ = nullptr;
    const ai::CharacterSimState* character_ = nullptr;
};

class MoodInspectorPanel {
public:
    void set_character(const ai::CharacterSimState& character);
    [[nodiscard]] ai::Mood mood() const { return mood_; }
    [[nodiscard]] f32 stress() const { return stress_; }
    [[nodiscard]] f32 energy() const { return energy_; }

private:
    ai::Mood mood_ = ai::Mood::Neutral;
    f32 stress_ = 0.0f;
    f32 energy_ = 1.0f;
};

class RomanceScheduleEditorPanel {
public:
    void set_schedule(const ai::DailySchedule& schedule);
    bool add_entry(const ai::ScheduleEntry& entry);
    [[nodiscard]] const ai::DailySchedule& schedule() const { return schedule_; }
    [[nodiscard]] const ai::ScheduleEntry* resolve_at(f32 hour,
                                                      const ai::ScheduleAdaptationContext& ctx) const;

private:
    ai::DailySchedule schedule_;
    ai::AdaptiveScheduleAI schedule_ai_;
};

class EventTemplateEditorPanel {
public:
    void set_events(const ai::RomanceEventSystem& events);
    [[nodiscard]] std::size_t template_count() const { return template_count_; }
    void select_event(std::string_view event_id) { selected_event_id_ = std::string(event_id); }
    [[nodiscard]] const std::string& selected_event_id() const { return selected_event_id_; }

private:
    std::size_t template_count_ = 0;
    std::string selected_event_id_;
};

class BoundaryConsentEditorPanel {
public:
    void set_character(const ai::CharacterSimState& character);
    [[nodiscard]] std::size_t boundary_count() const { return boundary_count_; }
    ai::ConsentCheckResult preview_action(std::string_view action_id,
                                          ai::RelationshipStage stage) const;

private:
    ai::CharacterSimState character_;
    ai::BoundaryConsentSystem boundaries_;
    std::size_t boundary_count_ = 0;
};

class OutfitReactionEditorPanel {
public:
    void set_system(const ai::OutfitReactionSystem& reactions);
    [[nodiscard]] std::size_t reaction_count() const { return reaction_count_; }

private:
    std::size_t reaction_count_ = 0;
};

class RoomBehaviorEditorPanel {
public:
    void set_system(const ai::RoomBehaviorSystem& room_behavior);
    [[nodiscard]] std::size_t action_count() const { return action_count_; }
    [[nodiscard]] std::size_t actions_for_room(std::string_view room_id) const;

private:
    const ai::RoomBehaviorSystem* room_behavior_ = nullptr;
    std::size_t action_count_ = 0;
};

} // namespace eve::editor
