#pragma once

#include <eve/ai/relationship.hpp>
#include <eve/ai/schedule.hpp>
#include <eve/content/content_types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::content {

struct AppearancePresetRef {
    std::string face_preset_id;
    std::string eye_preset_id;
    std::string hair_preset_id;
    std::string body_preset_id;
    std::string material_preset_id;
};

struct CharacterContentDefinition {
    ContentMetadata metadata;
    std::string biography;
    ai::PersonalityTraits personality;
    ai::CharacterPreferences preferences;
    AppearancePresetRef appearance;
    std::string voice_profile_id;
    std::string conversation_style;
    std::string schedule_id;
    std::vector<std::string> dialogue_package_ids;
    std::vector<std::string> animation_preset_ids;
    std::string idle_behavior_id;
    std::vector<std::string> relationship_ids;
};

struct AppearancePresetLibrary {
    std::vector<ContentAsset> face_presets;
    std::vector<ContentAsset> eye_presets;
    std::vector<ContentAsset> hair_presets;
    std::vector<ContentAsset> outfit_presets;
    std::vector<ContentAsset> makeup_presets;
    std::vector<ContentAsset> body_presets;
    std::vector<ContentAsset> material_presets;
    std::vector<ContentAsset> accessory_presets;
    std::vector<ContentAsset> pose_presets;
    std::vector<ContentAsset> expression_presets;
};

class CharacterContentLibrary {
public:
    bool load_character(std::string_view path);
    bool save_character(std::string_view path, const CharacterContentDefinition& def) const;
    void register_character(CharacterContentDefinition def);
    [[nodiscard]] CharacterContentDefinition* find(std::string_view id);
    [[nodiscard]] const CharacterContentDefinition* find(std::string_view id) const;
    [[nodiscard]] std::size_t character_count() const { return characters_.size(); }

    AppearancePresetLibrary& appearance_presets() { return appearance_; }
    [[nodiscard]] const AppearancePresetLibrary& appearance_presets() const { return appearance_; }

private:
    std::unordered_map<std::string, CharacterContentDefinition> characters_;
    AppearancePresetLibrary appearance_;
};

} // namespace eve::content
