#pragma once

#include <eve/human/body_regions.hpp>
#include <eve/human/interaction_rules.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::human {

struct BodyRegionAction {
    std::string id;
    std::string label;
    BodyRegion region = BodyRegion::Torso;
    std::string animation_id;
    std::string dialogue_id;
    std::string pose_preview_id;
    InteractionPermissionRequirements requirements;
    std::vector<std::string> tags;
};

struct BodyInteractionDefinition {
    std::string id;
    std::string display_name;
    BodyRegion region = BodyRegion::Torso;
    std::vector<BodyRegionAction> actions;
};

class BodyInteractionRegistry {
public:
    void register_definition(BodyInteractionDefinition definition);
    bool load_from_file(std::string_view path);
    bool load_from_directory(std::string_view directory);
    [[nodiscard]] const BodyInteractionDefinition* find(std::string_view id) const;
    [[nodiscard]] const BodyRegionAction* find_action(std::string_view action_id) const;
    [[nodiscard]] std::vector<BodyRegionAction> actions_for_region(BodyRegion region) const;
    [[nodiscard]] std::size_t definition_count() const { return definitions_.size(); }
    [[nodiscard]] std::size_t action_count() const;

private:
    std::unordered_map<std::string, BodyInteractionDefinition> definitions_;
};

struct BodyInteractionResult {
    bool success = false;
    BodyRegionAction action;
    InteractionRuleResult rule_result;
    std::string message;
};

class BodyInteractionSystem {
public:
    explicit BodyInteractionSystem(BodyInteractionRegistry& registry);

    [[nodiscard]] BodyRegionTarget query_region(const std::vector<BodyRegionVolume>& volumes,
                                                const math::Vec3& ray_origin,
                                                const math::Vec3& ray_direction) const;

    BodyInteractionResult execute(std::string_view action_id, const InteractionContextSnapshot& context);
    [[nodiscard]] std::vector<BodyRegionAction> available_actions(BodyRegion region,
                                                                  const InteractionContextSnapshot& context) const;

    void set_debug_visualization(bool enabled) { debug_visualization_ = enabled; }
    [[nodiscard]] bool debug_visualization() const { return debug_visualization_; }

private:
    BodyInteractionRegistry& registry_;
    InteractionRuleEngine rules_;
    bool debug_visualization_ = false;
};

} // namespace eve::human
