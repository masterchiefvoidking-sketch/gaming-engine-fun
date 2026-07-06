#include <eve/rainy_demo/rainy_evening_demo.hpp>
#include <eve/ai/boundaries.hpp>

namespace eve::rainy_demo {

bool RainyEveningDemo::initialize(std::string_view data_root) {
    data_root_ = std::string(data_root);
    if (!simulation_.initialize(data_root_)) {
        return false;
    }
    ai::CharacterSimState* character = simulation_.character();
    if (character != nullptr) {
        character->current_room_id = "living_room";
        character->current_outfit_id = "cozy_sweater";
    }
    initialized_ = true;
    return true;
}

void RainyEveningDemo::shutdown() {
    history_.clear();
    initialized_ = false;
}

DemoStepResult RainyEveningDemo::record(std::string_view action,
                                        const ai::InteractionResult& result) {
    DemoStepResult step;
    step.action = std::string(action);
    step.success = result.success;
    step.dialogue = result.dialogue_line;
    step.rejection = result.rejection_reason;
    step.stage = ai::to_string(result.new_stage);
    step.mood = ai::to_string(result.new_mood);
    history_.push_back(step);
    return step;
}

DemoStepResult RainyEveningDemo::talk_in_living_room() {
    return record("talk", simulation_.talk("rainy_evening_start"));
}

DemoStepResult RainyEveningDemo::offer_compliment() {
    return record("compliment", simulation_.offer_compliment());
}

DemoStepResult RainyEveningDemo::invite_to_dinner() {
    ai::CharacterSimState* character = simulation_.character();
    if (character != nullptr) {
        character->current_room_id = "dining_room";
    }
    return record("invite_dinner", simulation_.invite_to_dinner());
}

DemoStepResult RainyEveningDemo::trigger_outfit_dialogue() {
    return record("outfit_dialogue", simulation_.trigger_outfit_dialogue());
}

DemoStepResult RainyEveningDemo::trigger_rainy_event() {
    return record("rainy_event", simulation_.trigger_event("rainy_day_conversation"));
}

DemoStepResult RainyEveningDemo::try_blocked_action(std::string_view action_id) {
    ai::CharacterSimState* character = simulation_.character();
    if (character == nullptr) {
        return {};
    }
    ai::BoundaryConsentSystem boundaries;
    const ai::ConsentCheckResult consent =
        boundaries.check_action(action_id, *character, simulation_.current_stage());
    DemoStepResult step;
    step.action = std::string(action_id);
    step.success = consent.allowed;
    step.rejection = consent.rejection_line;
    step.stage = ai::to_string(simulation_.current_stage());
    if (character != nullptr) {
        step.mood = ai::to_string(character->mood);
    }
    history_.push_back(step);
    return step;
}

DemoStepResult RainyEveningDemo::recall_memory(std::string_view tag) {
    const ai::RelationshipModel* rel = simulation_.relationship();
    DemoStepResult step;
    step.action = "recall_" + std::string(tag);
    if (rel == nullptr) {
        step.rejection = "No relationship";
        history_.push_back(step);
        return step;
    }
    ai::RelationshipModel mutable_rel = *rel;
    const ai::MemoryRecallResult recall = simulation_.memories().recall_by_tag(mutable_rel, tag);
    step.success = recall.found;
    step.dialogue = recall.callback_line;
    history_.push_back(step);
    return step;
}

bool RainyEveningDemo::save_state(std::string_view path) const {
    return simulation_.save_state(path);
}

bool RainyEveningDemo::load_state(std::string_view path) {
    return simulation_.load_state(path);
}

std::string RainyEveningDemo::summarize() const {
    const ai::CharacterSimState* character = simulation_.character();
    const ai::RelationshipModel* rel = simulation_.relationship();
    if (character == nullptr || rel == nullptr) {
        return "Uninitialized";
    }
    return std::string("Aiko — ") + ai::to_string(simulation_.current_stage()) + ", mood " +
           ai::to_string(character->mood) + ", trust " + std::to_string(character->trust) +
           ", affection " + std::to_string(character->affection) + ", memories " +
           std::to_string(rel->shared_memories.size());
}

} // namespace eve::rainy_demo
