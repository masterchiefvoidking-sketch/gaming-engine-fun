#include <algorithm>
#include <eve/ai/romance_events.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::ai {

bool RomanceEventSystem::load_templates(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            RomanceEventTemplate tmpl;
            tmpl.id = entry.value("id", "");
            tmpl.label = entry.value("label", "");
            tmpl.dialogue_id = entry.value("dialogue_id", "");
            tmpl.memory_template_id = entry.value("memory_template", "");
            tmpl.required_room = entry.value("room", "");
            tmpl.trust_reward = entry.value("trust_reward", 0.05f);
            tmpl.affection_reward = entry.value("affection_reward", 0.08f);
            tmpl.one_time = entry.value("one_time", true);
            register_template(std::move(tmpl));
        }
        return !templates_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void RomanceEventSystem::register_template(RomanceEventTemplate tmpl) {
    templates_[tmpl.id] = std::move(tmpl);
}

bool RomanceEventSystem::was_completed(const RelationshipModel& relationship,
                                       std::string_view event_id) const {
    for (const MemoryEntry& mem : relationship.shared_memories) {
        if (mem.id == event_id) {
            return true;
        }
        for (const std::string& tag : mem.tags) {
            if (tag == event_id) {
                return true;
            }
        }
    }
    return false;
}

RomanceEventResult RomanceEventSystem::trigger(std::string_view event_id,
                                               CharacterSimState& character,
                                               RelationshipModel& relationship,
                                               const MemorySystem& memory, MoodSystem& mood) {
    RomanceEventResult result;
    const auto it = templates_.find(std::string(event_id));
    if (it == templates_.end()) {
        result.message = "Event not found";
        return result;
    }
    const RomanceEventTemplate& tmpl = it->second;

    if (tmpl.one_time && was_completed(relationship, event_id)) {
        result.already_completed = true;
        result.message = "Event already completed";
        return result;
    }
    if (!tmpl.required_room.empty() && character.current_room_id != tmpl.required_room) {
        result.message = "Wrong room for event";
        return result;
    }

    relationship.adjust_scores(tmpl.trust_reward, tmpl.affection_reward, 0.0f, 0.0f);
    character.trust = std::clamp(character.trust + tmpl.trust_reward, 0.0f, 1.0f);
    character.affection = std::clamp(character.affection + tmpl.affection_reward, 0.0f, 1.0f);

    if (tmpl.type == RomanceEventType::FirstSharedMeal ||
        tmpl.id.find("meal") != std::string::npos) {
        mood.apply_shared_meal(character);
    } else if (tmpl.type == RomanceEventType::Apology ||
               tmpl.id.find("comfort") != std::string::npos) {
        character.comfort = std::clamp(character.comfort + 0.1f, 0.0f, 1.0f);
    } else {
        mood.apply_compliment(character, 0.05f);
    }

    if (!tmpl.memory_template_id.empty()) {
        MemoryEntry mem =
            memory.create_memory(tmpl.memory_template_id, "player", character.familiarity > 0 ? 1 : 0);
        mem.tags.push_back(std::string(event_id));
        relationship.add_memory(std::move(mem));
        result.memory_created = relationship.shared_memories.back();
    }

    result.triggered = true;
    result.dialogue_id = tmpl.dialogue_id;
    result.message = tmpl.label;
    return result;
}

} // namespace eve::ai
