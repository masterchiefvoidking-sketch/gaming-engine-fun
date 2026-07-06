#include <eve/ai/relationship_simulation.hpp>
#include <eve/ai/relationship_stages.hpp>
#include <eve/ai/wardrobe.hpp>
#include <eve/platform/unified_save.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <chrono>
#include <nlohmann/json.hpp>

namespace eve::platform {

bool UnifiedSaveSystem::save(std::string_view path, const UnifiedSaveGame& save) const {
    if (!validate(save)) {
        return false;
    }
    nlohmann::json json;
    json["magic"] = kUnifiedSaveMagic;
    json["version"] = save.version;
    json["project_id"] = save.project_id;
    json["save_slot"] = save.save_slot;
    json["timestamp"] = save.timestamp;

    json["character"] = {{"id", save.character.character_id},
                         {"trust", save.character.trust},
                         {"affection", save.character.affection},
                         {"comfort", save.character.comfort},
                         {"mood", save.character.mood},
                         {"room", save.character.room},
                         {"outfit", save.character.outfit}};

    json["relationship"] = {{"trust", save.relationship.trust},
                            {"affection", save.relationship.affection},
                            {"respect", save.relationship.respect},
                            {"stage", save.relationship.stage}};

    json["dialogue"]["unlocked_nodes"] = save.dialogue.unlocked_nodes;
    json["dialogue"]["story_flags"] = save.dialogue.story_flags;
    json["dialogue"]["completed"] = save.dialogue.completed_conversations;

    json["memories"] = nlohmann::json::array();
    for (const MemorySaveEntry& mem : save.memories) {
        json["memories"].push_back(
            {{"id", mem.id}, {"description", mem.description}, {"weight", mem.weight}});
    }

    json["apartment"] = {{"id", save.apartment.apartment_id},
                         {"room", save.apartment.active_room},
                         {"time", save.apartment.time_of_day},
                         {"weather", save.apartment.weather},
                         {"day", save.apartment.day_index}};

    json["inventory"]["items"] = save.inventory.item_ids;

    json["wardrobe"] = {{"active", save.wardrobe.active_outfit_id},
                        {"owned", save.wardrobe.owned_outfit_ids},
                        {"favorites", save.wardrobe.favorites}};

    json["photos"] = nlohmann::json::array();
    for (const PhotoSaveEntry& photo : save.photos) {
        json["photos"].push_back(
            {{"id", photo.id}, {"path", photo.path}, {"caption", photo.caption}, {"day", photo.day_index}});
    }

    json["story"]["completed_events"] = save.story.completed_events;
    json["story"]["completed_scenes"] = save.story.completed_scenes;
    json["story"]["chapters"] = save.story.unlocked_chapters;

    json["settings"] = {{"master_volume", save.settings.master_volume},
                        {"music_volume", save.settings.music_volume},
                        {"voice_volume", save.settings.voice_volume},
                        {"locale", save.settings.locale},
                        {"quality", save.settings.quality_preset},
                        {"touch_controls", save.settings.touch_controls}};

    return FileSystem::write_text_file(path, json.dump(2));
}

bool UnifiedSaveSystem::load(std::string_view path, UnifiedSaveGame& save) const {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        if (json.value("magic", "") != kUnifiedSaveMagic) {
            return false;
        }
        save.version = json.value("version", kUnifiedSaveVersion);
        save.project_id = json.value("project_id", "");
        save.save_slot = json.value("save_slot", "slot_01");
        save.timestamp = json.value("timestamp", 0ULL);

        if (json.contains("character")) {
            save.character.character_id = json["character"].value("id", "");
            save.character.trust = json["character"].value("trust", 0.0f);
            save.character.affection = json["character"].value("affection", 0.0f);
            save.character.comfort = json["character"].value("comfort", 0.0f);
            save.character.mood = json["character"].value("mood", "neutral");
            save.character.room = json["character"].value("room", "");
            save.character.outfit = json["character"].value("outfit", "");
        }
        if (json.contains("relationship")) {
            save.relationship.trust = json["relationship"].value("trust", 0.0f);
            save.relationship.affection = json["relationship"].value("affection", 0.0f);
            save.relationship.respect = json["relationship"].value("respect", 0.0f);
            save.relationship.stage = json["relationship"].value("stage", "stranger");
        }
        if (json.contains("dialogue")) {
            save.dialogue.unlocked_nodes =
                json["dialogue"].value("unlocked_nodes", std::vector<std::string>{});
            save.dialogue.story_flags =
                json["dialogue"].value("story_flags", std::vector<std::string>{});
            save.dialogue.completed_conversations =
                json["dialogue"].value("completed", std::vector<std::string>{});
        }
        if (json.contains("memories")) {
            save.memories.clear();
            for (const auto& mem : json["memories"]) {
                MemorySaveEntry entry;
                entry.id = mem.value("id", "");
                entry.description = mem.value("description", "");
                entry.weight = mem.value("weight", 0.5f);
                save.memories.push_back(std::move(entry));
            }
        }
        if (json.contains("apartment")) {
            save.apartment.apartment_id = json["apartment"].value("id", "");
            save.apartment.active_room = json["apartment"].value("room", "");
            save.apartment.time_of_day = json["apartment"].value("time", 12.0f);
            save.apartment.weather = json["apartment"].value("weather", "clear");
            save.apartment.day_index = json["apartment"].value("day", 0u);
        }
        if (json.contains("inventory")) {
            save.inventory.item_ids =
                json["inventory"].value("items", std::vector<std::string>{});
        }
        if (json.contains("wardrobe")) {
            save.wardrobe.active_outfit_id = json["wardrobe"].value("active", "");
            save.wardrobe.owned_outfit_ids =
                json["wardrobe"].value("owned", std::vector<std::string>{});
            save.wardrobe.favorites =
                json["wardrobe"].value("favorites", std::vector<std::string>{});
        }
        if (json.contains("photos")) {
            save.photos.clear();
            for (const auto& photo : json["photos"]) {
                PhotoSaveEntry entry;
                entry.id = photo.value("id", "");
                entry.path = photo.value("path", "");
                entry.caption = photo.value("caption", "");
                entry.day_index = photo.value("day", 0u);
                save.photos.push_back(std::move(entry));
            }
        }
        if (json.contains("story")) {
            save.story.completed_events =
                json["story"].value("completed_events", std::vector<std::string>{});
            save.story.completed_scenes =
                json["story"].value("completed_scenes", std::vector<std::string>{});
            save.story.unlocked_chapters =
                json["story"].value("chapters", std::vector<std::string>{});
        }
        if (json.contains("settings")) {
            save.settings.master_volume = json["settings"].value("master_volume", 1.0f);
            save.settings.music_volume = json["settings"].value("music_volume", 0.7f);
            save.settings.voice_volume = json["settings"].value("voice_volume", 1.0f);
            save.settings.locale = json["settings"].value("locale", "en");
            save.settings.quality_preset = json["settings"].value("quality", 2u);
            save.settings.touch_controls = json["settings"].value("touch_controls", false);
        }
        return validate(save);
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool UnifiedSaveSystem::validate(const UnifiedSaveGame& save) const {
    return save.version > 0 && save.version <= kUnifiedSaveVersion && !save.project_id.empty();
}

void UnifiedSaveSystem::apply_to_relationship_sim(ai::RelationshipSimulation& sim,
                                                const UnifiedSaveGame& save) const {
    ai::CharacterSimState* character = sim.character();
    if (character != nullptr) {
        character->trust = save.character.trust;
        character->affection = save.character.affection;
        character->comfort = save.character.comfort;
        character->current_room_id = save.character.room;
        character->current_outfit_id = save.character.outfit;
    }
    ai::RelationshipModel* rel = sim.relationship();
    if (rel != nullptr) {
        rel->scores.trust = save.relationship.trust;
        rel->scores.affection = save.relationship.affection;
        rel->scores.respect = save.relationship.respect;
        rel->shared_memories.clear();
        for (const MemorySaveEntry& mem : save.memories) {
            ai::MemoryEntry entry;
            entry.id = mem.id;
            entry.description = mem.description;
            entry.emotional_weight = mem.weight;
            entry.tags = {mem.id};
            rel->add_memory(std::move(entry));
        }
    }
}

UnifiedSaveGame UnifiedSaveSystem::capture_from_relationship_sim(
    const ai::RelationshipSimulation& sim, std::string_view project_id) const {
    UnifiedSaveGame save;
    save.project_id = std::string(project_id);
    save.timestamp = static_cast<u64>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());

    const ai::CharacterSimState* character = sim.character();
    if (character != nullptr) {
        save.character.character_id = character->character_id;
        save.character.trust = character->trust;
        save.character.affection = character->affection;
        save.character.comfort = character->comfort;
        save.character.mood = ai::to_string(character->mood);
        save.character.room = character->current_room_id;
        save.character.outfit = character->current_outfit_id;
        save.wardrobe.active_outfit_id = character->current_outfit_id;
    }

    const ai::RelationshipModel* rel = sim.relationship();
    if (rel != nullptr) {
        save.relationship.trust = rel->scores.trust;
        save.relationship.affection = rel->scores.affection;
        save.relationship.respect = rel->scores.respect;
        save.relationship.stage = ai::to_string(sim.current_stage());
        for (const ai::MemoryEntry& mem : rel->shared_memories) {
            save.memories.push_back({mem.id, mem.description, mem.emotional_weight});
        }
    }
    return save;
}

} // namespace eve::platform
