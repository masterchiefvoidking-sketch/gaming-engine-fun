#include <eve/content/character_content.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::content {

bool CharacterContentLibrary::load_character(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        CharacterContentDefinition def;
        def.metadata.id = json.value("id", "");
        def.metadata.display_name = json.value("name", "");
        def.biography = json.value("biography", "");
        def.voice_profile_id = json.value("voice_profile", "");
        def.conversation_style = json.value("conversation_style", "warm");
        def.schedule_id = json.value("schedule", "");
        def.idle_behavior_id = json.value("idle_behavior", "neutral_idle");
        if (json.contains("personality")) {
            def.personality.openness = json["personality"].value("openness", 0.5f);
            def.personality.extraversion = json["personality"].value("extraversion", 0.5f);
            def.personality.agreeableness = json["personality"].value("agreeableness", 0.5f);
        }
        if (json.contains("appearance")) {
            def.appearance.face_preset_id = json["appearance"].value("face", "");
            def.appearance.eye_preset_id = json["appearance"].value("eyes", "");
            def.appearance.hair_preset_id = json["appearance"].value("hair", "");
            def.appearance.body_preset_id = json["appearance"].value("body", "");
        }
        if (json.contains("dialogue_packages")) {
            for (const auto& pkg : json["dialogue_packages"]) {
                def.dialogue_package_ids.push_back(pkg.get<std::string>());
            }
        }
        register_character(std::move(def));
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool CharacterContentLibrary::save_character(std::string_view path,
                                             const CharacterContentDefinition& def) const {
    nlohmann::json json;
    json["id"] = def.metadata.id;
    json["name"] = def.metadata.display_name;
    json["biography"] = def.biography;
    json["voice_profile"] = def.voice_profile_id;
    json["conversation_style"] = def.conversation_style;
    json["schedule"] = def.schedule_id;
    json["idle_behavior"] = def.idle_behavior_id;
    json["personality"] = {{"openness", def.personality.openness},
                           {"extraversion", def.personality.extraversion},
                           {"agreeableness", def.personality.agreeableness}};
    json["appearance"] = {{"face", def.appearance.face_preset_id},
                          {"eyes", def.appearance.eye_preset_id},
                          {"hair", def.appearance.hair_preset_id},
                          {"body", def.appearance.body_preset_id}};
    json["dialogue_packages"] = def.dialogue_package_ids;
    return FileSystem::write_text_file(path, json.dump(2));
}

void CharacterContentLibrary::register_character(CharacterContentDefinition def) {
    characters_[def.metadata.id] = std::move(def);
}

CharacterContentDefinition* CharacterContentLibrary::find(std::string_view id) {
    const auto it = characters_.find(std::string(id));
    if (it == characters_.end()) {
        return nullptr;
    }
    return &it->second;
}

const CharacterContentDefinition* CharacterContentLibrary::find(std::string_view id) const {
    const auto it = characters_.find(std::string(id));
    if (it == characters_.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace eve::content
