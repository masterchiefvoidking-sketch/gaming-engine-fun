#pragma once

#include <eve/scene/scene_conditions.hpp>
#include <eve/scene/scene_types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::scene {

struct RoomTriggerDefinition {
    std::string id;
    std::string room_id;
    std::string scene_template_id;
    std::string condition_id;
    std::string interaction_id;
};

struct RoomTriggerResult {
    bool triggered = false;
    std::string scene_id;
    std::string rejection_reason;
};

class RoomEventTriggerSystem {
public:
    bool load_triggers(std::string_view path);
    void register_trigger(RoomTriggerDefinition trigger);

    [[nodiscard]] std::vector<RoomTriggerDefinition> triggers_for_room(
        std::string_view room_id) const;
    RoomTriggerResult evaluate_trigger(std::string_view trigger_id,
                                       const SceneRuntimeContext& ctx,
                                       const SceneConditionEvaluator& conditions) const;
    RoomTriggerResult find_eligible(std::string_view room_id, std::string_view interaction_id,
                                    const SceneRuntimeContext& ctx,
                                    const SceneConditionEvaluator& conditions) const;

    [[nodiscard]] std::size_t trigger_count() const { return triggers_.size(); }

private:
    std::vector<RoomTriggerDefinition> triggers_;
};

} // namespace eve::scene
