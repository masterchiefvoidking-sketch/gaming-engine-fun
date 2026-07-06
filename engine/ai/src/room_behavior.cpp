#include <eve/ai/room_behavior.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::ai {

bool RoomBehaviorSystem::load_from_file(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            RoomBehaviorAction action;
            action.id = entry.value("id", "");
            action.room_id = entry.value("room_id", "");
            action.label = entry.value("label", "");
            action.dialogue_id = entry.value("dialogue_id", "");
            action.interaction_id = entry.value("interaction_id", "");
            register_action(std::move(action));
        }
        return !actions_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void RoomBehaviorSystem::register_action(RoomBehaviorAction action) {
    actions_.push_back(std::move(action));
}

std::vector<RoomBehaviorAction> RoomBehaviorSystem::actions_for_room(
    std::string_view room_id, const CharacterSimState& character,
    RelationshipStage stage) const {
    std::vector<RoomBehaviorAction> result;
    for (const RoomBehaviorAction& action : actions_) {
        if (action.room_id != room_id) {
            continue;
        }
        if (minimum_stage_level(stage) < minimum_stage_level(action.min_stage)) {
            continue;
        }
        if (static_cast<u8>(character.privacy_level) < static_cast<u8>(action.required_privacy)) {
            continue;
        }
        result.push_back(action);
    }
    return result;
}

const RoomBehaviorAction* RoomBehaviorSystem::pick_best(std::string_view room_id,
                                                        const CharacterSimState& character,
                                                        RelationshipStage stage) const {
    const auto actions = actions_for_room(room_id, character, stage);
    if (actions.empty()) {
        return nullptr;
    }
    return &actions[0];
}

} // namespace eve::ai
