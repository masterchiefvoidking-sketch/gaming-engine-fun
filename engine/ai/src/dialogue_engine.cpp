#include <algorithm>
#include <eve/ai/dialogue_engine.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::ai {

namespace {

DialogueConditions parse_conditions(const nlohmann::json& json) {
    DialogueConditions cond;
    if (json.contains("min_stage")) {
        const std::string stage = json["min_stage"].get<std::string>();
        if (stage == "friend") cond.min_stage = RelationshipStage::Friend;
        else if (stage == "close_friend") cond.min_stage = RelationshipStage::CloseFriend;
        else if (stage == "romantic_interest") cond.min_stage = RelationshipStage::RomanticInterest;
        else if (stage == "partner") cond.min_stage = RelationshipStage::Partner;
        else if (stage == "acquaintance") cond.min_stage = RelationshipStage::Acquaintance;
    }
    cond.min_trust = json.value("min_trust", 0.0f);
    cond.min_affection = json.value("min_affection", 0.0f);
    if (json.contains("rooms")) {
        for (const auto& r : json["rooms"]) {
            cond.required_rooms.push_back(r.get<std::string>());
        }
    }
    if (json.contains("memories")) {
        for (const auto& m : json["memories"]) {
            cond.required_memories.push_back(m.get<std::string>());
        }
    }
    if (json.contains("time_of_day")) {
        for (const auto& t : json["time_of_day"]) {
            cond.time_of_day.push_back(t.get<std::string>());
        }
    }
    if (json.contains("outfit_tags")) {
        for (const auto& t : json["outfit_tags"]) {
            cond.required_outfit_tags.push_back(t.get<std::string>());
        }
    }
    return cond;
}

} // namespace

bool DialogueEngine::load_from_file(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            DialogueNode node;
            node.id = entry.value("id", "");
            node.speaker = entry.value("speaker", "");
            node.line = entry.value("line", "");
            node.category = entry.value("category", "general");
            node.default_next = entry.value("next", "");
            if (entry.contains("conditions")) {
                node.conditions = parse_conditions(entry["conditions"]);
            }
            if (entry.contains("choices")) {
                for (const auto& ch : entry["choices"]) {
                    DialogueChoice choice;
                    choice.id = ch.value("id", "");
                    choice.label = ch.value("label", "");
                    choice.effect.trust_delta = ch.value("trust_delta", 0.0f);
                    choice.effect.affection_delta = ch.value("affection_delta", 0.0f);
                    choice.effect.comfort_delta = ch.value("comfort_delta", 0.0f);
                    choice.effect.memory_tag = ch.value("memory_tag", "");
                    choice.effect.next_node_id = ch.value("next", "");
                    node.choices.push_back(std::move(choice));
                }
            }
            register_node(std::move(node));
        }
        return !nodes_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void DialogueEngine::register_node(DialogueNode node) {
    nodes_[node.id] = std::move(node);
}

DialogueResult DialogueEngine::evaluate_node(std::string_view node_id,
                                             const DialogueContext& context) const {
    DialogueResult result;
    const auto it = nodes_.find(std::string(node_id));
    if (it == nodes_.end()) {
        result.rejection_reason = "Node not found";
        return result;
    }
    if (!conditions_met(it->second.conditions, context)) {
        result.rejection_reason = "Conditions not met";
        return result;
    }
    result.success = true;
    result.node = &it->second;
    for (const DialogueChoice& choice : it->second.choices) {
        result.available_choices.push_back(choice);
    }
    return result;
}

std::vector<const DialogueNode*> DialogueEngine::matching_lines(
    const DialogueContext& context, std::string_view category) const {
    std::vector<const DialogueNode*> matches;
    for (const auto& [_, node] : nodes_) {
        if (node.category != category) {
            continue;
        }
        if (conditions_met(node.conditions, context)) {
            matches.push_back(&node);
        }
    }
    return matches;
}

bool DialogueEngine::conditions_met(const DialogueConditions& conditions,
                                    const DialogueContext& context) const {
    if (context.character == nullptr) {
        return false;
    }
    if (minimum_stage_level(context.stage) < minimum_stage_level(conditions.min_stage)) {
        return false;
    }
    if (context.character->trust < conditions.min_trust) {
        return false;
    }
    if (context.character->affection < conditions.min_affection) {
        return false;
    }
    if (!conditions.required_rooms.empty()) {
        bool room_ok = false;
        for (const std::string& room : conditions.required_rooms) {
            if (room == context.active_room) {
                room_ok = true;
                break;
            }
        }
        if (!room_ok) {
            return false;
        }
    }
    if (!conditions.required_memories.empty()) {
        for (const std::string& mem : conditions.required_memories) {
            bool found = false;
            for (const std::string& known : context.known_memory_tags) {
                if (known == mem) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
    }
    if (!conditions.time_of_day.empty()) {
        bool time_ok = false;
        for (const std::string& bucket : conditions.time_of_day) {
            if (time_matches(context.time_of_day, bucket)) {
                time_ok = true;
                break;
            }
        }
        if (!time_ok) {
            return false;
        }
    }
    return true;
}

DialogueResult DialogueEngine::apply_choice(const DialogueChoice& choice,
                                            CharacterSimState& character) const {
    character.trust = std::clamp(character.trust + choice.effect.trust_delta, 0.0f, 1.0f);
    character.affection = std::clamp(character.affection + choice.effect.affection_delta, 0.0f, 1.0f);
    character.comfort = std::clamp(character.comfort + choice.effect.comfort_delta, 0.0f, 1.0f);
    DialogueResult result;
    if (!choice.effect.next_node_id.empty()) {
        DialogueContext ctx;
        ctx.character = &character;
        return evaluate_node(choice.effect.next_node_id, ctx);
    }
    result.success = true;
    return result;
}

bool DialogueEngine::time_matches(f32 hour, std::string_view bucket) const {
    if (bucket == "morning") return hour >= 6.0f && hour < 12.0f;
    if (bucket == "afternoon") return hour >= 12.0f && hour < 17.0f;
    if (bucket == "evening") return hour >= 17.0f && hour < 21.0f;
    if (bucket == "night") return hour >= 21.0f || hour < 6.0f;
    return true;
}

} // namespace eve::ai
