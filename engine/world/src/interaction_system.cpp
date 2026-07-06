#include <eve/core/filesystem/filesystem.hpp>
#include <eve/world/interaction_system.hpp>
#include <nlohmann/json.hpp>

namespace eve::world {

namespace {

ContextAction parse_action(const nlohmann::json& json) {
    ContextAction action;
    action.id = json.value("id", "");
    action.label = json.value("label", action.id);
    action.category = interaction_category_from_string(json.value("category", "general"));
    action.animation_id = json.value("animation", "");
    action.sound_id = json.value("sound", "");
    action.resulting_state = json.value("resulting_state", "");
    action.duration_seconds = json.value("duration", 0.0f);
    action.ai_usable = json.value("ai_usable", true);
    action.requires_empty_hands = json.value("requires_empty_hands", false);
    if (json.contains("required_items")) {
        for (const auto& item : json["required_items"]) {
            action.required_items.push_back(item.get<std::string>());
        }
    }
    if (json.contains("tags")) {
        for (const auto& tag : json["tags"]) {
            action.tags.push_back(tag.get<std::string>());
        }
    }
    return action;
}

InteractionDefinition parse_definition(const nlohmann::json& json) {
    InteractionDefinition def;
    def.id = json.value("id", "");
    def.display_name = json.value("display_name", def.id);
    def.room = room_type_from_string(json.value("room", "hallway"));
    def.default_state = json.value("default_state", "idle");
    if (json.contains("supported_states")) {
        for (const auto& state : json["supported_states"]) {
            def.supported_states.push_back(state.get<std::string>());
        }
    }
    if (json.contains("actions")) {
        for (const auto& action_json : json["actions"]) {
            def.actions.push_back(parse_action(action_json));
        }
    }
    return def;
}

ObjectState state_from_string(std::string_view value) {
    if (value == "in_use")
        return ObjectState::InUse;
    if (value == "dirty")
        return ObjectState::Dirty;
    if (value == "broken")
        return ObjectState::Broken;
    if (value == "off")
        return ObjectState::Off;
    if (value == "on")
        return ObjectState::On;
    if (value == "open")
        return ObjectState::Open;
    if (value == "closed")
        return ObjectState::Closed;
    return ObjectState::Idle;
}

} // namespace

void InteractionRegistry::register_definition(InteractionDefinition definition) {
    definitions_[definition.id] = std::move(definition);
}

const InteractionDefinition* InteractionRegistry::find(std::string_view id) const {
    const auto it = definitions_.find(std::string(id));
    if (it == definitions_.end()) {
        return nullptr;
    }
    return &it->second;
}

std::vector<std::string> InteractionRegistry::definition_ids() const {
    std::vector<std::string> ids;
    ids.reserve(definitions_.size());
    for (const auto& [id, _] : definitions_) {
        ids.push_back(id);
    }
    return ids;
}

bool InteractionRegistry::load_from_file(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        if (json.is_array()) {
            for (const auto& entry : json) {
                register_definition(parse_definition(entry));
            }
        } else {
            register_definition(parse_definition(json));
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool InteractionRegistry::load_from_directory(std::string_view directory) {
    if (!FileSystem::is_directory(directory)) {
        return false;
    }
    bool loaded_any = false;
    for (const std::string& file : FileSystem::list_directory(directory)) {
        if (file.size() >= 5 && file.substr(file.size() - 5) == ".json") {
            const std::string path = std::string(directory) + "/" + file;
            loaded_any = load_from_file(path) || loaded_any;
        }
    }
    return loaded_any;
}

InteractionSystem::InteractionSystem(InteractionRegistry& registry)
    : registry_(registry) {}

HoverResult InteractionSystem::query_hover(ecs::World& world, const math::Vec3& point,
                                           f32 radius) const {
    HoverResult result;
    const f32 radius_sq = radius * radius;

    world.each([&](EntityId entity, ecs::EntityRecord&) {
        auto* interactable = world.get_component<InteractableComponent>(entity);
        if (interactable == nullptr || !interactable->hoverable) {
            return;
        }
        const math::Vec3 delta = interactable->position - point;
        const f32 dist_sq = delta.dot(delta);
        if (dist_sq > radius_sq) {
            return;
        }
        if (!result.valid ||
            dist_sq < (world.get_component<InteractableComponent>(result.entity)->position - point)
                          .dot(world.get_component<InteractableComponent>(result.entity)->position -
                               point)) {
            result.entity = entity;
            result.interactable_name = interactable->display_name;
            result.valid = true;
        }
    });

    if (result.valid) {
        result.available_actions = available_actions(world, result.entity, kInvalidEntity);
    }
    return result;
}

InteractionResult InteractionSystem::execute_action(ecs::World& world, EntityId interactable,
                                                    EntityId actor, std::string_view action_id) {
    InteractionResult result;
    auto* component = world.get_component<InteractableComponent>(interactable);
    if (component == nullptr) {
        result.message = "Interactable not found";
        return result;
    }

    const InteractionDefinition* def = registry_.find(component->definition_id);
    if (def == nullptr) {
        result.message = "Definition not found";
        return result;
    }

    const ContextAction* action = nullptr;
    for (const ContextAction& candidate : def->actions) {
        if (candidate.id == action_id) {
            action = &candidate;
            break;
        }
    }
    if (action == nullptr) {
        result.message = "Action not found";
        return result;
    }
    if (!can_execute(*action, *component, actor)) {
        result.message = "Action not available";
        return result;
    }

    component->state = ObjectState::InUse;
    component->current_user = actor;
    if (!action->resulting_state.empty()) {
        component->state = state_from_string(action->resulting_state);
        component->custom_state["last_action"] = action->id;
    }

    result.success = true;
    result.action_id = action->id;
    result.animation_id = action->animation_id;
    result.sound_id = action->sound_id;
    result.new_state = component->state;
    result.duration_seconds = action->duration_seconds;
    result.message = "Executed " + action->label;
    return result;
}

std::vector<ContextAction> InteractionSystem::available_actions(ecs::World& world,
                                                                EntityId interactable,
                                                                EntityId actor) const {
    std::vector<ContextAction> actions;
    const auto* component = world.get_component<InteractableComponent>(interactable);
    if (component == nullptr) {
        return actions;
    }
    const InteractionDefinition* def = registry_.find(component->definition_id);
    if (def == nullptr) {
        return actions;
    }
    for (const ContextAction& action : def->actions) {
        if (can_execute(action, *component, actor)) {
            actions.push_back(action);
        }
    }
    return actions;
}

void InteractionSystem::set_hovered(ecs::World& world, EntityId interactable, bool hovered) {
    if (auto* component = world.get_component<InteractableComponent>(interactable)) {
        component->hovered = hovered;
    }
}

void InteractionSystem::release_interactable(ecs::World& world, EntityId interactable) {
    if (auto* component = world.get_component<InteractableComponent>(interactable)) {
        component->current_user = kInvalidEntity;
        if (component->state == ObjectState::InUse) {
            component->state = ObjectState::Idle;
        }
    }
}

bool InteractionSystem::can_execute(const ContextAction& action,
                                    const InteractableComponent& target, EntityId actor) const {
    (void)actor;
    if (target.current_user != kInvalidEntity && target.current_user != actor) {
        return false;
    }
    if (target.state == ObjectState::Broken) {
        return false;
    }
    return true;
}

} // namespace eve::world
