#pragma once

#include <eve/scene/room_event_triggers.hpp>
#include <eve/scene/scene_conditions.hpp>
#include <eve/scene/scene_types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::scene {

struct ScheduledEventCandidate {
    std::string scene_id;
    f32 priority = 0.0f;
    std::string reason;
};

struct SchedulerContext {
    SceneRuntimeContext runtime;
    f32 current_hour = 18.0f;
    std::unordered_map<std::string, f32> cooldowns;
};

class EventScheduler {
public:
    bool load_templates(std::string_view path);
    void register_template(SceneTemplate tmpl);

    [[nodiscard]] std::vector<ScheduledEventCandidate> eligible_events(
        const SchedulerContext& ctx, const SceneConditionEvaluator& conditions) const;
    [[nodiscard]] bool is_on_cooldown(std::string_view scene_id,
                                      const SchedulerContext& ctx) const;
    void set_cooldown(std::string_view scene_id, f32 hours, SchedulerContext& ctx) const;

    [[nodiscard]] const SceneTemplate* find_template(std::string_view scene_id) const;
    [[nodiscard]] std::size_t template_count() const { return templates_.size(); }

private:
    std::unordered_map<std::string, SceneTemplate> templates_;
};

} // namespace eve::scene
