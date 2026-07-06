#include <eve/editor/romance_sim_tools.hpp>
#include <sstream>

namespace eve::editor {

void DialogueGraphEditorPanel::set_engine(const ai::DialogueEngine& engine) {
    node_count_ = engine.node_count();
}

void MemoryDebuggerPanel::set_system(const ai::MemorySystem& memories,
                                     const ai::RelationshipModel& relationship) {
    memories_ = &memories;
    relationship_ = &relationship;
    active_memory_count_ = relationship.shared_memories.size();
    template_count_ = 0;
}

void MemoryDebuggerPanel::recall(std::string_view tag) {
    if (memories_ == nullptr || relationship_ == nullptr) {
        return;
    }
    ai::RelationshipModel mutable_rel = *relationship_;
    const ai::MemoryRecallResult result = memories_->recall_by_tag(mutable_rel, tag);
    last_recall_line_ = result.found ? result.callback_line : "No memory found";
}

void RelationshipInspectorPanel::set_simulation(const ai::RelationshipSimulation& simulation) {
    relationship_ = simulation.relationship();
    character_ = simulation.character();
    stage_ = simulation.current_stage();
}

std::string RelationshipInspectorPanel::summary() const {
    std::ostringstream out;
    if (character_ != nullptr) {
        out << "Stage: " << ai::to_string(stage_) << ", Trust: " << character_->trust
            << ", Affection: " << character_->affection;
    }
    if (relationship_ != nullptr) {
        out << ", Memories: " << relationship_->shared_memories.size();
    }
    return out.str();
}

void MoodInspectorPanel::set_character(const ai::CharacterSimState& character) {
    mood_ = character.mood;
    stress_ = character.stress;
    energy_ = character.energy;
}

void RomanceScheduleEditorPanel::set_schedule(const ai::DailySchedule& schedule) {
    schedule_ = schedule;
}

bool RomanceScheduleEditorPanel::add_entry(const ai::ScheduleEntry& entry) {
    schedule_.entries.push_back(entry);
    return true;
}

const ai::ScheduleEntry* RomanceScheduleEditorPanel::resolve_at(
    f32 hour, const ai::ScheduleAdaptationContext& ctx) const {
    return schedule_ai_.resolve_activity(schedule_, hour, ctx);
}

void EventTemplateEditorPanel::set_events(const ai::RomanceEventSystem& events) {
    template_count_ = events.template_count();
}

void BoundaryConsentEditorPanel::set_character(const ai::CharacterSimState& character) {
    character_ = character;
    boundary_count_ = character.boundaries.size();
}

ai::ConsentCheckResult BoundaryConsentEditorPanel::preview_action(
    std::string_view action_id, ai::RelationshipStage stage) const {
    return boundaries_.check_action(action_id, character_, stage);
}

void OutfitReactionEditorPanel::set_system(const ai::OutfitReactionSystem& reactions) {
    reaction_count_ = reactions.reaction_count();
}

void RoomBehaviorEditorPanel::set_system(const ai::RoomBehaviorSystem& room_behavior) {
    room_behavior_ = &room_behavior;
    action_count_ = room_behavior.action_count();
}

std::size_t RoomBehaviorEditorPanel::actions_for_room(std::string_view room_id) const {
    if (room_behavior_ == nullptr) {
        return 0;
    }
    ai::CharacterSimState dummy;
    return room_behavior_->actions_for_room(room_id, dummy, ai::RelationshipStage::Friend).size();
}

} // namespace eve::editor
