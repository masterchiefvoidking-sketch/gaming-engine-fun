#pragma once

#include <eve/scene/scene_types.hpp>

#include <string>
#include <unordered_map>

namespace eve::scene {

struct ConditionEvaluationResult {
    bool eligible = false;
    std::string rejection_reason;
};

class SceneConditionEvaluator {
public:
    bool load_conditions(std::string_view path);
    void register_condition(EventConditionDefinition condition);

    [[nodiscard]] ConditionEvaluationResult evaluate(std::string_view condition_id,
                                                     const SceneRuntimeContext& ctx) const;
    [[nodiscard]] bool time_matches(f32 hour, std::string_view bucket) const;
    [[nodiscard]] std::size_t condition_count() const { return conditions_.size(); }

private:
    [[nodiscard]] ConditionEvaluationResult evaluate_definition(
        const EventConditionDefinition& cond, const SceneRuntimeContext& ctx) const;

    std::unordered_map<std::string, EventConditionDefinition> conditions_;
};

} // namespace eve::scene
