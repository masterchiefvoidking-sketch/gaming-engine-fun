#include <eve/core/filesystem/filesystem.hpp>
#include <eve/human/body_interaction_system.hpp>

#include <nlohmann/json.hpp>

namespace eve::human {

namespace {

InteractionPermissionRequirements parse_requirements(const nlohmann::json& json) {
    InteractionPermissionRequirements req;
    req.min_trust = json.value("min_trust", 0.0f);
    req.min_affection = json.value("min_affection", 0.0f);
    req.min_relationship = json.value("min_relationship", 0.0f);
    if (json.contains("allowed_rooms")) {
        for (const auto& room : json["allowed_rooms"]) {
            req.allowed_rooms.push_back(room.get<std::string>());
        }
    }
    return req;
}

BodyRegionAction parse_action(const nlohmann::json& json) {
    BodyRegionAction action;
    action.id = json.value("id", "");
    action.label = json.value("label", action.id);
    action.region = body_region_from_string(json.value("region", "torso"));
    action.animation_id = json.value("animation", "");
    action.dialogue_id = json.value("dialogue", "");
    action.pose_preview_id = json.value("pose_preview", "");
    if (json.contains("requirements")) {
        action.requirements = parse_requirements(json["requirements"]);
    }
    return action;
}

} // namespace

void BodyInteractionRegistry::register_definition(BodyInteractionDefinition definition) {
    definitions_[definition.id] = std::move(definition);
}

bool BodyInteractionRegistry::load_from_file(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        if (json.is_array()) {
            for (const auto& entry : json) {
                BodyInteractionDefinition def;
                def.id = entry.value("id", "");
                def.display_name = entry.value("display_name", def.id);
                def.region = body_region_from_string(entry.value("region", "torso"));
                if (entry.contains("actions")) {
                    for (const auto& action_json : entry["actions"]) {
                        def.actions.push_back(parse_action(action_json));
                    }
                }
                register_definition(std::move(def));
            }
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool BodyInteractionRegistry::load_from_directory(std::string_view directory) {
    if (!FileSystem::is_directory(directory)) {
        return false;
    }
    bool loaded = false;
    for (const std::string& file : FileSystem::list_directory(directory)) {
        if (file.size() >= 5 && (file.ends_with(".json") || file.ends_with(".yaml"))) {
            loaded = load_from_file(std::string(directory) + "/" + file) || loaded;
        }
    }
    return loaded;
}

const BodyInteractionDefinition* BodyInteractionRegistry::find(std::string_view id) const {
    const auto it = definitions_.find(std::string(id));
    if (it == definitions_.end()) {
        return nullptr;
    }
    return &it->second;
}

const BodyRegionAction* BodyInteractionRegistry::find_action(std::string_view action_id) const {
    for (const auto& [_, def] : definitions_) {
        for (const BodyRegionAction& action : def.actions) {
            if (action.id == action_id) {
                return &action;
            }
        }
    }
    return nullptr;
}

std::vector<BodyRegionAction> BodyInteractionRegistry::actions_for_region(BodyRegion region) const {
    std::vector<BodyRegionAction> actions;
    for (const auto& [_, def] : definitions_) {
        if (def.region == region) {
            actions.insert(actions.end(), def.actions.begin(), def.actions.end());
        }
    }
    return actions;
}

std::size_t BodyInteractionRegistry::action_count() const {
    std::size_t count = 0;
    for (const auto& [_, def] : definitions_) {
        count += def.actions.size();
    }
    return count;
}

BodyInteractionSystem::BodyInteractionSystem(BodyInteractionRegistry& registry) : registry_(registry) {}

BodyRegionTarget BodyInteractionSystem::query_region(const std::vector<BodyRegionVolume>& volumes,
                                                     const math::Vec3& ray_origin,
                                                     const math::Vec3& ray_direction) const {
    BodyRegionTarget best;
    f32 best_distance = 1e9f;

    for (const BodyRegionVolume& volume : volumes) {
        if (!volume.hoverable) {
            continue;
        }
        const math::Vec3 to_center = volume.center - ray_origin;
        const f32 projection = to_center.dot(ray_direction.normalized());
        if (projection < 0.0f) {
            continue;
        }
        const math::Vec3 closest = ray_origin + ray_direction.normalized() * projection;
        const math::Vec3 delta = closest - volume.center;
        if (std::abs(delta.x) <= volume.half_extents.x && std::abs(delta.y) <= volume.half_extents.y &&
            std::abs(delta.z) <= volume.half_extents.z && projection < best_distance) {
            best_distance = projection;
            best.region = volume.region;
            best.region_id = volume.id;
            best.hovered = true;
            best.distance = projection;
        }
    }
    return best;
}

BodyInteractionResult BodyInteractionSystem::execute(std::string_view action_id,
                                                     const InteractionContextSnapshot& context) {
    BodyInteractionResult result;
    const BodyRegionAction* action = registry_.find_action(action_id);
    if (action == nullptr) {
        result.message = "Action not found";
        return result;
    }
    result.rule_result = rules_.evaluate(action->requirements, context);
    if (!result.rule_result.allowed) {
        result.message = result.rule_result.reason;
        return result;
    }
    result.success = true;
    result.action = *action;
    result.message = "Executed " + action->label;
    return result;
}

std::vector<BodyRegionAction> BodyInteractionSystem::available_actions(
    BodyRegion region, const InteractionContextSnapshot& context) const {
    std::vector<BodyRegionAction> available;
    for (const BodyRegionAction& action : registry_.actions_for_region(region)) {
        if (rules_.evaluate(action.requirements, context).allowed) {
            available.push_back(action);
        }
    }
    return available;
}

} // namespace eve::human
