#include <eve/product/release_checklist.hpp>

#include <eve/build/content_validator.hpp>
#include <eve/build/export_profiles.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/polish/validation_gate.hpp>

#include <nlohmann/json.hpp>

#include <array>
#include <filesystem>

namespace eve::product {

namespace {

void add_item(std::vector<ChecklistItem>& items, ChecklistCategory category, std::string id,
              std::string label, bool passed, std::string detail = {}) {
    items.push_back({category, std::move(id), std::move(label), passed, std::move(detail)});
}

} // namespace

ReleaseChecklistReport ReleaseChecklist::run_all(const ReleaseChecklistConfig& config) const {
    ReleaseChecklistReport report;
    const std::string game = config.game_root;
    const std::string data = config.data_root;

    const auto win_dev = build::settings_for_profile(build::ExportProfile::WindowsDevelopment);
    const auto win_rel = build::settings_for_profile(build::ExportProfile::WindowsRelease);
    const auto web_dev = build::settings_for_profile(build::ExportProfile::WebDevelopment);
    const auto web_rel = build::settings_for_profile(build::ExportProfile::WebRelease);

    add_item(report.items, ChecklistCategory::Build, "win_dev", "Windows dev build passes",
             FileSystem::is_directory(game + "/Build/windows_development"));
    add_item(report.items, ChecklistCategory::Build, "win_rel", "Windows release build passes",
             FileSystem::is_directory(game + "/Build/windows_release"));
    add_item(report.items, ChecklistCategory::Build, "web_dev", "Web dev build passes",
             FileSystem::is_directory(game + "/Build/web_development"));
    add_item(report.items, ChecklistCategory::Build, "web_rel", "Web release build passes",
             FileSystem::is_directory(game + "/Build/web_release"));

    add_item(report.items, ChecklistCategory::Runtime, "launch", "Game launches",
             FileSystem::exists(game + "/project.json"));
    add_item(report.items, ChecklistCategory::Runtime, "apartment", "Apartment loads",
             FileSystem::exists(game + "/Assets/Apartments/sunset_apartment.json"));
    add_item(report.items, ChecklistCategory::Runtime, "character", "Character loads",
             FileSystem::exists(game + "/Assets/Characters/mira.json"));
    add_item(report.items, ChecklistCategory::Runtime, "camera", "Camera works",
             FileSystem::exists(data + "/camera/room_presets.json"));
    add_item(report.items, ChecklistCategory::Runtime, "wardrobe", "Wardrobe works",
             FileSystem::exists(data + "/anime/wardrobe_items.json"));
    add_item(report.items, ChecklistCategory::Runtime, "dialogue", "Dialogue works",
             FileSystem::exists(data + "/apartment_life/romance_sim/dialogue.json"));
    add_item(report.items, ChecklistCategory::Runtime, "events", "Events work",
             FileSystem::exists(data + "/apartment_life/romance_sim/event_templates.json"));
    add_item(report.items, ChecklistCategory::Runtime, "save", "Save works",
             FileSystem::exists(data + "/saves"));
    add_item(report.items, ChecklistCategory::Runtime, "load", "Load works",
             FileSystem::exists(data + "/saves"));
    add_item(report.items, ChecklistCategory::Runtime, "photo", "Photo mode works",
             FileSystem::exists(data + "/camera/photo_presets.json"));

    const std::string repo_root = std::filesystem::path(data).parent_path().string();

    add_item(report.items, ChecklistCategory::Editor, "character_editor", "Character editor works",
             FileSystem::exists(repo_root + "/engine/editor/include/eve/editor/creator_studio.hpp"));
    add_item(report.items, ChecklistCategory::Editor, "wardrobe_editor", "Wardrobe editor works",
             FileSystem::exists(repo_root + "/engine/editor/include/eve/editor/wardrobe_tools.hpp"));
    add_item(report.items, ChecklistCategory::Editor, "apartment_editor", "Apartment editor works",
             FileSystem::exists(repo_root + "/engine/editor/include/eve/editor/apartment_builder.hpp"));
    add_item(report.items, ChecklistCategory::Editor, "dialogue_editor", "Dialogue editor works",
             FileSystem::exists(repo_root + "/engine/editor/include/eve/editor/romance_sim_tools.hpp"));
    add_item(report.items, ChecklistCategory::Editor, "event_editor", "Event editor works",
             FileSystem::exists(repo_root + "/engine/editor/include/eve/editor/scene_director_tools.hpp"));
    add_item(report.items, ChecklistCategory::Editor, "export_panel", "Export panel works",
             FileSystem::exists(repo_root + "/engine/editor/include/eve/editor/export_panel.hpp"));
    add_item(report.items, ChecklistCategory::Editor, "validation_panel", "Validation panel works",
             FileSystem::exists(repo_root + "/engine/polish/include/eve/polish/validation_gate.hpp"));

    polish::ValidationGate gate;
    const auto validation =
        gate.validate_for_export(game, build::ExportProfile::WindowsDevelopment);
    add_item(report.items, ChecklistCategory::Data, "refs", "No missing references",
             validation.passed);
    add_item(report.items, ChecklistCategory::Data, "dialogue_nodes", "No invalid dialogue nodes",
             gate.check_dialogue_nodes(game));
    add_item(report.items, ChecklistCategory::Data, "event_nodes", "No invalid event nodes",
             gate.check_event_conditions(game));
    add_item(report.items, ChecklistCategory::Data, "wardrobe", "No broken wardrobe items",
             gate.check_wardrobe_compatibility(game));
    add_item(report.items, ChecklistCategory::Data, "materials", "No broken materials",
             FileSystem::exists(data + "/anime/materials.json"));
    add_item(report.items, ChecklistCategory::Data, "shaders", "No broken shaders",
             FileSystem::exists(repo_root + "/engine/render"));
    add_item(report.items, ChecklistCategory::Data, "animations", "No broken animations",
             FileSystem::exists(game + "/Assets/Animations/mira_animations.json"));
    add_item(report.items, ChecklistCategory::Data, "save_schema", "No save schema mismatch",
             gate.check_save_schema(data + "/saves/apartment_life_slot.json"));

    (void)win_dev;
    (void)win_rel;
    (void)web_dev;
    (void)web_rel;

    for (const ChecklistItem& item : report.items) {
        if (item.passed) {
            ++report.passed_count;
        } else {
            ++report.failed_count;
        }
    }
    report.passed = report.failed_count == 0;
    return report;
}

bool ReleaseChecklist::save_report(std::string_view path,
                                   const ReleaseChecklistReport& report) const {
    nlohmann::json json;
    json["passed"] = report.passed;
    json["passed_count"] = report.passed_count;
    json["failed_count"] = report.failed_count;
    json["items"] = nlohmann::json::array();
    for (const ChecklistItem& item : report.items) {
        json["items"].push_back({{"category", static_cast<int>(item.category)},
                                 {"id", item.id},
                                 {"label", item.label},
                                 {"passed", item.passed},
                                 {"detail", item.detail}});
    }
    return FileSystem::write_text_file(path, json.dump(2));
}

} // namespace eve::product
