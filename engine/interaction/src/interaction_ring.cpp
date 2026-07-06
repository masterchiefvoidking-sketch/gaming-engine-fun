#include <eve/interaction/interaction_ring.hpp>

namespace eve::interaction {

namespace {

InteractionRingAction make_action(RingActionType type, std::string id, std::string label,
                                  bool enabled, std::string reason = {}) {
    InteractionRingAction action;
    action.type = type;
    action.id = std::move(id);
    action.label = std::move(label);
    action.enabled = enabled;
    action.disabled_reason = std::move(reason);
    return action;
}

bool room_allows(const InteractionRingContext& context, const std::vector<std::string>& rooms) {
    if (rooms.empty()) {
        return true;
    }
    for (const std::string& room : rooms) {
        if (room == context.interaction.room_id) {
            return true;
        }
    }
    return false;
}

} // namespace

bool InteractionRing::is_action_available(RingActionType type,
                                          const InteractionRingContext& context) const {
    switch (type) {
    case RingActionType::Look:
        return context.is_character || context.is_object;
    case RingActionType::Talk:
        return context.is_character;
    case RingActionType::Compliment:
        return context.is_character && context.interaction.mood != "upset";
    case RingActionType::ChangeOutfit:
        return context.is_character && context.interaction.privacy_level >= 0.5f;
    case RingActionType::Pose:
        return context.is_character && context.interaction.relationship >= 0.15f;
    case RingActionType::UseObject:
        return context.is_object && context.current_activity != "busy";
    case RingActionType::Inspect:
        return context.is_character && context.interaction.player_permission;
    case RingActionType::Cancel:
        return true;
    }
    return false;
}

InteractionRingAction InteractionRing::action_for_type(RingActionType type,
                                                       const InteractionRingContext& context) const {
    const bool enabled = is_action_available(type, context);
    std::string reason;
    if (!enabled) {
        if (type == RingActionType::Compliment && context.interaction.mood == "upset") {
            reason = "Character is upset";
        } else if (type == RingActionType::ChangeOutfit && context.interaction.privacy_level < 0.5f) {
            reason = "Not enough privacy";
        } else if (type == RingActionType::Talk && !context.is_character) {
            reason = "No one to talk to";
        } else {
            reason = "Unavailable in current context";
        }
    }

    switch (type) {
    case RingActionType::Look:
        return make_action(type, "look", "Look", enabled, reason);
    case RingActionType::Talk:
        return make_action(type, "talk", "Talk", enabled, reason);
    case RingActionType::Compliment:
        return make_action(type, "compliment", "Compliment", enabled, reason);
    case RingActionType::ChangeOutfit:
        return make_action(type, "change_outfit", "Change Outfit", enabled, reason);
    case RingActionType::Pose:
        return make_action(type, "pose", "Pose", enabled, reason);
    case RingActionType::UseObject:
        return make_action(type, "use_object", "Use", enabled, reason);
    case RingActionType::Inspect:
        return make_action(type, "inspect", "Inspect", enabled, reason);
    case RingActionType::Cancel:
        return make_action(type, "cancel", "Cancel", true);
    }
    return make_action(RingActionType::Cancel, "cancel", "Cancel", true);
}

std::vector<InteractionRingAction>
InteractionRing::build_actions(const InteractionRingContext& context) const {
    std::vector<InteractionRingAction> actions;
    const RingActionType types[] = {RingActionType::Look,     RingActionType::Talk,
                                    RingActionType::Compliment, RingActionType::ChangeOutfit,
                                    RingActionType::Pose,     RingActionType::UseObject,
                                    RingActionType::Inspect,  RingActionType::Cancel};
    for (RingActionType type : types) {
        InteractionRingAction action = action_for_type(type, context);
        if (type != RingActionType::Cancel || actions.empty()) {
            actions.push_back(std::move(action));
        }
    }
    (void)room_allows;
    return actions;
}

} // namespace eve::interaction
