#include <algorithm>
#include <eve/ai/social_ai.hpp>

namespace eve::ai {

SocialAI::SocialAI(SocialContext* context)
    : context_(context) {}

std::vector<SocialAction> SocialAI::evaluate_character(EntityId character,
                                                       const CharacterState& state) const {
    std::vector<SocialAction> actions;
    auto append = [&](std::vector<SocialAction> source) {
        actions.insert(actions.end(), source.begin(), source.end());
    };
    append(schedule_actions(character, state));
    append(relationship_actions(character, state));
    append(mood_actions(character, state));
    return actions;
}

void SocialAI::react_to_event(EntityId character, EntityId other, const std::string& event_id) {
    if (context_ == nullptr || context_->relationships == nullptr) {
        return;
    }
    RelationshipModel& relationship = context_->relationships->get_or_create(character, other);
    if (event_id == "shared_meal") {
        relationship.adjust_scores(0.02f, 0.05f, 0.01f, 0.02f);
        relationship.add_shared_experience("Shared a meal together");
    } else if (event_id == "comfort") {
        relationship.adjust_scores(0.05f, 0.04f, 0.02f, 0.0f);
        relationship.add_memory(
            {"comfort", "Offered comfort", std::to_string(other), 0.6f, 0, {"social"}});
    } else if (event_id == "celebration") {
        relationship.adjust_scores(0.03f, 0.06f, 0.02f, 0.05f);
        relationship.add_shared_experience("Celebrated an event together");
    }
}

SocialAction SocialAI::pick_best_action(const std::vector<SocialAction>& actions) const {
    if (actions.empty()) {
        return {};
    }
    return *std::max_element(
        actions.begin(), actions.end(),
        [](const SocialAction& a, const SocialAction& b) { return a.priority < b.priority; });
}

std::vector<SocialAction> SocialAI::schedule_actions(EntityId character,
                                                     const CharacterState& state) const {
    (void)state;
    std::vector<SocialAction> actions;
    if (context_ == nullptr || context_->schedules == nullptr) {
        return actions;
    }
    const DailySchedule* schedule = context_->schedules->find(std::to_string(character));
    if (schedule == nullptr) {
        return actions;
    }
    const ScheduleEntry* entry = schedule->active_entry(context_->current_hour);
    if (entry == nullptr) {
        return actions;
    }
    SocialAction action;
    action.intent = SocialIntent::InviteActivity;
    action.label = entry->label;
    action.target_room = entry->room_id;
    action.interaction_id = entry->interaction_id;
    action.initiator = character;
    action.priority = static_cast<f32>(entry->priority);
    actions.push_back(action);
    return actions;
}

std::vector<SocialAction> SocialAI::relationship_actions(EntityId character,
                                                         const CharacterState& state) const {
    (void)state;
    std::vector<SocialAction> actions;
    if (context_ == nullptr || context_->relationships == nullptr) {
        return actions;
    }

    SocialAction conversation;
    conversation.intent = SocialIntent::StartConversation;
    conversation.label = "Start a conversation";
    conversation.initiator = character;
    conversation.priority = 0.4f;
    actions.push_back(conversation);

    SocialAction meal;
    meal.intent = SocialIntent::ShareMeal;
    meal.label = "Invite to share a meal";
    meal.target_room = "kitchen";
    meal.interaction_id = "cook_meal";
    meal.initiator = character;
    meal.priority = 0.5f;
    actions.push_back(meal);

    return actions;
}

std::vector<SocialAction> SocialAI::mood_actions(EntityId character,
                                                 const CharacterState& state) const {
    std::vector<SocialAction> actions;
    if (state.mood == Mood::Sad || state.stress > 0.7f) {
        SocialAction comfort;
        comfort.intent = SocialIntent::Comfort;
        comfort.label = "Seek comfort";
        comfort.initiator = character;
        comfort.priority = 0.8f;
        actions.push_back(comfort);
    }
    if (state.mood == Mood::Playful || state.mood == Mood::Happy) {
        SocialAction media;
        media.intent = SocialIntent::WatchMedia;
        media.label = "Watch a movie together";
        media.target_room = "living_room";
        media.interaction_id = "watch_tv";
        media.initiator = character;
        media.priority = 0.6f;
        actions.push_back(media);
    }
    if (state.energy > 0.6f) {
        SocialAction exercise;
        exercise.intent = SocialIntent::Exercise;
        exercise.label = "Exercise together";
        exercise.target_room = "balcony";
        exercise.interaction_id = "exercise";
        exercise.initiator = character;
        exercise.priority = 0.3f;
        actions.push_back(exercise);
    }
    return actions;
}

} // namespace eve::ai
