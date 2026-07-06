#include <eve/ai/memory_system.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::ai {

bool MemorySystem::load_templates(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            MemoryTemplate tmpl;
            tmpl.id = entry.value("id", "");
            tmpl.description_template = entry.value("description", "");
            tmpl.default_emotional_weight = entry.value("emotional_weight", 0.5f);
            if (entry.contains("callbacks")) {
                for (const auto& cb : entry["callbacks"]) {
                    tmpl.dialogue_callbacks.push_back(cb.get<std::string>());
                }
            }
            register_template(std::move(tmpl));
        }
        return !templates_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void MemorySystem::register_template(MemoryTemplate tmpl) {
    templates_[tmpl.id] = std::move(tmpl);
}

MemoryEntry MemorySystem::create_memory(std::string_view template_id, std::string_view related_character,
                                        u32 day_index, const std::string& detail) const {
    MemoryEntry entry;
    const auto it = templates_.find(std::string(template_id));
    if (it != templates_.end()) {
        entry.id = it->second.id;
        entry.description = detail.empty() ? it->second.description_template : detail;
        entry.emotional_weight = it->second.default_emotional_weight;
        entry.tags = {std::string(template_id)};
    } else {
        entry.id = std::string(template_id);
        entry.description = detail;
        entry.tags = {std::string(template_id)};
    }
    entry.related_character = std::string(related_character);
    entry.day_index = day_index;
    return entry;
}

MemoryRecallResult MemorySystem::recall_by_tag(RelationshipModel& relationship,
                                               std::string_view tag) const {
    MemoryRecallResult result;
    for (const MemoryEntry& mem : relationship.shared_memories) {
        if (mem.id == tag) {
            result.found = true;
            result.entry = mem;
            const auto tmpl_it = templates_.find(mem.id);
            if (tmpl_it != templates_.end() && !tmpl_it->second.dialogue_callbacks.empty()) {
                result.callback_line = tmpl_it->second.dialogue_callbacks[0];
            } else {
                result.callback_line = "I still remember when " + mem.description;
            }
            return result;
        }
        for (const std::string& mem_tag : mem.tags) {
            if (mem_tag == tag) {
                result.found = true;
                result.entry = mem;
                const auto tmpl_it = templates_.find(mem.id);
                if (tmpl_it != templates_.end() && !tmpl_it->second.dialogue_callbacks.empty()) {
                    result.callback_line = tmpl_it->second.dialogue_callbacks[0];
                } else {
                    result.callback_line = "I still remember when " + mem.description;
                }
                return result;
            }
        }
    }
    return result;
}

std::vector<std::string> MemorySystem::all_memory_tags(const RelationshipModel& relationship) const {
    std::vector<std::string> tags;
    for (const MemoryEntry& mem : relationship.shared_memories) {
        tags.push_back(mem.id);
        for (const std::string& t : mem.tags) {
            tags.push_back(t);
        }
    }
    return tags;
}

bool MemorySystem::has_memory(const RelationshipModel& relationship, std::string_view tag) const {
    for (const MemoryEntry& mem : relationship.shared_memories) {
        if (mem.id == tag) {
            return true;
        }
        for (const std::string& t : mem.tags) {
            if (t == tag) {
                return true;
            }
        }
    }
    return false;
}

} // namespace eve::ai
