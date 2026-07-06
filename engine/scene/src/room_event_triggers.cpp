#include <eve/scene/room_event_triggers.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::scene {

bool RoomEventTriggerSystem::load_triggers(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            RoomTriggerDefinition trigger;
            trigger.id = entry.value("id", "");
            trigger.room_id = entry.value("room", "");
            trigger.scene_template_id = entry.value("scene", "");
            trigger.condition_id = entry.value("condition", "");
            trigger.interaction_id = entry.value("interaction", "");
            register_trigger(std::move(trigger));
        }
        return !triggers_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void RoomEventTriggerSystem::register_trigger(RoomTriggerDefinition trigger) {
    triggers_.push_back(std::move(trigger));
}

std::vector<RoomTriggerDefinition> RoomEventTriggerSystem::triggers_for_room(
    std::string_view room_id) const {
    std::vector<RoomTriggerDefinition> result;
    for (const RoomTriggerDefinition& trigger : triggers_) {
        if (trigger.room_id == room_id) {
            result.push_back(trigger);
        }
    }
    return result;
}

RoomTriggerResult RoomEventTriggerSystem::evaluate_trigger(
    std::string_view trigger_id, const SceneRuntimeContext& ctx,
    const SceneConditionEvaluator& conditions) const {
    RoomTriggerResult result;
    for (const RoomTriggerDefinition& trigger : triggers_) {
        if (trigger.id != trigger_id) {
            continue;
        }
        if (!trigger.condition_id.empty()) {
            const ConditionEvaluationResult cond = conditions.evaluate(trigger.condition_id, ctx);
            if (!cond.eligible) {
                result.rejection_reason = cond.rejection_reason;
                return result;
            }
        }
        result.triggered = true;
        result.scene_id = trigger.scene_template_id;
        return result;
    }
    result.rejection_reason = "Trigger not found";
    return result;
}

RoomTriggerResult RoomEventTriggerSystem::find_eligible(
    std::string_view room_id, std::string_view interaction_id, const SceneRuntimeContext& ctx,
    const SceneConditionEvaluator& conditions) const {
    for (const RoomTriggerDefinition& trigger : triggers_) {
        if (trigger.room_id != room_id) {
            continue;
        }
        if (!interaction_id.empty() && !trigger.interaction_id.empty() &&
            trigger.interaction_id != interaction_id) {
            continue;
        }
        if (!trigger.condition_id.empty()) {
            const ConditionEvaluationResult cond = conditions.evaluate(trigger.condition_id, ctx);
            if (!cond.eligible) {
                continue;
            }
        }
        RoomTriggerResult result;
        result.triggered = true;
        result.scene_id = trigger.scene_template_id;
        return result;
    }
    RoomTriggerResult result;
    result.rejection_reason = "No eligible room trigger";
    return result;
}

} // namespace eve::scene
