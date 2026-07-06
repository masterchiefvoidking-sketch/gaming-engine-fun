#pragma once

#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::ai {
class RelationshipSimulation;
}

namespace eve::platform {

constexpr u32 kUnifiedSaveVersion = 1;
constexpr const char* kUnifiedSaveMagic = "EVE_SAVE";

struct CharacterSaveSlice {
    std::string character_id;
    f32 trust = 0.0f;
    f32 affection = 0.0f;
    f32 comfort = 0.0f;
    std::string mood;
    std::string room;
    std::string outfit;
};

struct RelationshipSaveSlice {
    f32 trust = 0.0f;
    f32 affection = 0.0f;
    f32 respect = 0.0f;
    std::string stage;
};

struct MemorySaveEntry {
    std::string id;
    std::string description;
    f32 weight = 0.5f;
};

struct DialogueSaveSlice {
    std::vector<std::string> unlocked_nodes;
    std::vector<std::string> story_flags;
    std::vector<std::string> completed_conversations;
};

struct ApartmentSaveSlice {
    std::string apartment_id;
    std::string active_room;
    f32 time_of_day = 12.0f;
    std::string weather = "clear";
    u32 day_index = 0;
};

struct InventorySaveSlice {
    std::vector<std::string> item_ids;
};

struct WardrobeSaveSlice {
    std::string active_outfit_id;
    std::vector<std::string> owned_outfit_ids;
    std::vector<std::string> favorites;
};

struct PhotoSaveEntry {
    std::string id;
    std::string path;
    std::string caption;
    u32 day_index = 0;
};

struct StoryProgressSlice {
    std::vector<std::string> completed_events;
    std::vector<std::string> completed_scenes;
    std::vector<std::string> unlocked_chapters;
};

struct SettingsSaveSlice {
    f32 master_volume = 1.0f;
    f32 music_volume = 0.7f;
    f32 voice_volume = 1.0f;
    std::string locale = "en";
    u32 quality_preset = 2;
    bool touch_controls = false;
};

struct UnifiedSaveGame {
    u32 version = kUnifiedSaveVersion;
    std::string project_id;
    std::string save_slot = "slot_01";
    u64 timestamp = 0;
    CharacterSaveSlice character;
    RelationshipSaveSlice relationship;
    DialogueSaveSlice dialogue;
    std::vector<MemorySaveEntry> memories;
    ApartmentSaveSlice apartment;
    InventorySaveSlice inventory;
    WardrobeSaveSlice wardrobe;
    std::vector<PhotoSaveEntry> photos;
    StoryProgressSlice story;
    SettingsSaveSlice settings;
};

class UnifiedSaveSystem {
public:
    bool save(std::string_view path, const UnifiedSaveGame& save) const;
    bool load(std::string_view path, UnifiedSaveGame& save) const;
    [[nodiscard]] bool validate(const UnifiedSaveGame& save) const;

    void apply_to_relationship_sim(ai::RelationshipSimulation& sim,
                                   const UnifiedSaveGame& save) const;
    UnifiedSaveGame capture_from_relationship_sim(const ai::RelationshipSimulation& sim,
                                                  std::string_view project_id) const;
};

} // namespace eve::platform
