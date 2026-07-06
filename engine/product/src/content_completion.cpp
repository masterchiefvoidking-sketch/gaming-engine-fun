#include <eve/product/content_completion.hpp>

#include <eve/content/game_content_project.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::product {

namespace {

std::size_t count_json_array(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return 0;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        if (json.is_array()) {
            return json.size();
        }
        if (json.contains("clips")) {
            return json["clips"].size();
        }
        if (json.contains("presets")) {
            return json["presets"].size();
        }
    } catch (const nlohmann::json::exception&) {
    }
    return 0;
}

std::size_t count_directory_json(std::string_view dir) {
    if (!FileSystem::is_directory(dir)) {
        return 0;
    }
    std::size_t count = 0;
    for (const std::string& file : FileSystem::list_directory(dir)) {
        if (file.ends_with(".json")) {
            ++count;
        }
    }
    return count;
}

} // namespace

ContentCompletionReport ContentCompletionValidator::validate(std::string_view game_root,
                                                             std::string_view data_root) const {
    ContentCompletionReport report;
    const std::string game(game_root);
    const std::string data(data_root);

    content::GameContentProject project;
    if (!project.open(game_root)) {
        report.missing_requirements.push_back("Failed to open game project");
        return report;
    }

    report.wardrobe_items = static_cast<u32>(project.wardrobe().item_count());
    report.hairstyles = static_cast<u32>(
        count_directory_json(game + "/Assets/Characters/Hairstyles"));
    report.expressions = static_cast<u32>(
        count_directory_json(game + "/Assets/Characters/Expressions"));
    report.events = static_cast<u32>(project.events().event_count());

    const content::ApartmentContentDefinition* apartment =
        project.apartments().find_apartment("sunset_apartment");
    if (apartment != nullptr) {
        report.rooms = static_cast<u32>(apartment->room_ids.size());
    }

    report.object_interactions = static_cast<u32>(
        count_json_array(game + "/Assets/Interactions/apartment_interactions.json"));
    report.dialogue_lines = static_cast<u32>(
        count_json_array(data + "/apartment_life/romance_sim/dialogue.json"));
    report.camera_presets =
        static_cast<u32>(count_json_array(data + "/camera/room_presets.json"));
    report.lighting_presets =
        static_cast<u32>(count_json_array(data + "/anime/room_lighting.json"));
    report.photo_presets =
        static_cast<u32>(count_json_array(data + "/camera/photo_presets.json"));

    if (project.characters().character_count() < 1) {
        report.missing_requirements.push_back("Missing original character");
    }
    if (report.wardrobe_items < 20) {
        report.missing_requirements.push_back("Need at least 20 wardrobe items");
    }
    if (report.hairstyles < 5) {
        report.missing_requirements.push_back("Need at least 5 hairstyles");
    }
    if (report.expressions < 10) {
        report.missing_requirements.push_back("Need at least 10 expressions");
    }
    if (report.rooms < 6) {
        report.missing_requirements.push_back("Need at least 6 room areas");
    }
    if (report.object_interactions < 50) {
        report.missing_requirements.push_back("Need at least 50 object interactions");
    }
    if (report.dialogue_lines < 150) {
        report.missing_requirements.push_back("Need at least 150 dialogue lines");
    }
    if (report.events < 10) {
        report.missing_requirements.push_back("Need at least 10 playable events");
    }
    if (report.camera_presets < 12) {
        report.missing_requirements.push_back("Need at least 12 camera presets");
    }
    if (report.lighting_presets < 10) {
        report.missing_requirements.push_back("Need at least 10 lighting presets");
    }
    if (report.photo_presets < 8) {
        report.missing_requirements.push_back("Need at least 8 photo mode presets");
    }

    report.complete = report.missing_requirements.empty();
    return report;
}

} // namespace eve::product
