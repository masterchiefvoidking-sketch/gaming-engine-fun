#include <algorithm>
#include <eve/ai/character_sim_state.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::ai {

void CharacterSimState::record_interaction(std::string_view interaction_id) {
    recent_interactions.emplace_back(interaction_id);
    if (recent_interactions.size() > 20) {
        recent_interactions.erase(recent_interactions.begin());
    }
    familiarity = std::min(familiarity + 0.02f, 1.0f);
}

void CharacterSimState::apply_mood_delta(Mood new_mood, f32 intensity) {
    mood = new_mood;
    comfort = std::clamp(comfort + intensity * 0.1f, 0.0f, 1.0f);
    if (new_mood == Mood::Happy || new_mood == Mood::Romantic) {
        stress = std::max(0.0f, stress - intensity * 0.15f);
        confidence = std::min(confidence + intensity * 0.05f, 1.0f);
    } else if (new_mood == Mood::Anxious || new_mood == Mood::Stressed) {
        stress = std::min(stress + intensity * 0.2f, 1.0f);
        shyness = std::min(shyness + intensity * 0.1f, 1.0f);
    }
}

bool CharacterProfileLibrary::load_profile(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        CharacterSimState state;
        state.character_id = json.value("id", "");
        state.current_outfit_id = json.value("default_outfit", "");
        state.shyness = json.value("shyness", 0.5f);
        state.confidence = json.value("confidence", 0.5f);
        if (json.contains("personality")) {
            state.personality.openness = json["personality"].value("openness", 0.5f);
            state.personality.extraversion = json["personality"].value("extraversion", 0.5f);
            state.personality.agreeableness = json["personality"].value("agreeableness", 0.5f);
        }
        if (json.contains("preferences")) {
            for (const auto& room : json["preferences"].value("favorite_rooms", nlohmann::json::array())) {
                state.preferences.favorite_rooms.push_back(room.get<std::string>());
            }
            for (const auto& food : json["preferences"].value("favorite_foods", nlohmann::json::array())) {
                state.preferences.favorite_foods.push_back(food.get<std::string>());
            }
        }
        register_state(std::move(state));
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool CharacterProfileLibrary::load_boundaries(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        const std::string char_id = json.value("character_id", "");
        CharacterSimState* state = find(char_id);
        if (state == nullptr) {
            return false;
        }
        for (const auto& entry : json["boundaries"]) {
            BoundaryRule rule;
            rule.id = entry.value("id", "");
            rule.description = entry.value("description", "");
            if (entry.contains("blocked_actions")) {
                for (const auto& action : entry["blocked_actions"]) {
                    rule.blocked_actions.push_back(action.get<std::string>());
                }
            }
            state->boundaries.push_back(std::move(rule));
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void CharacterProfileLibrary::register_state(CharacterSimState state) {
    characters_[state.character_id] = std::move(state);
}

CharacterSimState* CharacterProfileLibrary::find(std::string_view character_id) {
    const auto it = characters_.find(std::string(character_id));
    if (it == characters_.end()) {
        return nullptr;
    }
    return &it->second;
}

const CharacterSimState* CharacterProfileLibrary::find(std::string_view character_id) const {
    const auto it = characters_.find(std::string(character_id));
    if (it == characters_.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace eve::ai
