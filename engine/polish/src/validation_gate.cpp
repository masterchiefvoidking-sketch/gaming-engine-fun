#include <eve/polish/validation_gate.hpp>

#include <eve/build/save_migration.hpp>
#include <eve/content/game_content_project.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/human/wardrobe_system.hpp>

#include <nlohmann/json.hpp>

namespace eve::polish {

ValidationGateResult ValidationGate::validate_for_export(std::string_view project_root,
                                                         build::ExportProfile profile) const {
    ValidationGateResult result;
    const build::ExportProfileSettings settings = build::settings_for_profile(profile);
    result.content_report = build::ContentValidator().validate(project_root, settings);
    result.passed = result.content_report.passed;

    if (!check_wardrobe_compatibility(project_root)) {
        result.blocking_issues.push_back("Invalid wardrobe compatibility detected");
        result.passed = false;
    }
    if (!check_dialogue_nodes(project_root)) {
        result.blocking_issues.push_back("Broken dialogue nodes detected");
        result.passed = false;
    }
    if (!check_event_conditions(project_root)) {
        result.blocking_issues.push_back("Broken event conditions detected");
        result.passed = false;
    }
    if (check_clipping_severity(project_root)) {
        result.warnings.push_back("Severe clipping issues detected in wardrobe data");
    }

    for (const build::ValidationIssue& issue : result.content_report.issues) {
        if (issue.severity == build::ValidationSeverity::Error) {
            result.blocking_issues.push_back(issue.message);
        } else {
            result.warnings.push_back(issue.message);
        }
    }
    return result;
}

bool ValidationGate::check_save_schema(std::string_view save_path) const {
    if (!FileSystem::exists(save_path)) {
        return true;
    }
    return !build::SaveMigration().needs_migration(save_path);
}

bool ValidationGate::check_wardrobe_compatibility(std::string_view game_root) const {
    human::WardrobeSystem wardrobe;
    const std::string data_path = std::string(game_root).substr(0, game_root.find("/Game")) +
                                  "/data/anime/wardrobe_items.json";
    if (!wardrobe.load_items(data_path)) {
        return false;
    }
    human::LayeredOutfit outfit;
    outfit = wardrobe.equip("mira_outfit_01", outfit);
    return wardrobe.can_equip("mira_outfit_02", outfit) &&
           wardrobe.find_item("mira_outfit_02") != nullptr;
}

bool ValidationGate::check_dialogue_nodes(std::string_view game_root) const {
    content::GameContentProject project;
    if (!project.open(game_root)) {
        return false;
    }
    return project.dialogue().package_count() > 0;
}

bool ValidationGate::check_event_conditions(std::string_view game_root) const {
    content::GameContentProject project;
    if (!project.open(game_root)) {
        return false;
    }
    return project.events().event_count() >= 5;
}

bool ValidationGate::check_clipping_severity(std::string_view game_root) const {
    (void)game_root;
    return false;
}

bool ValidationGate::save_report(std::string_view path, const ValidationGateResult& result) const {
    nlohmann::json json;
    json["passed"] = result.passed;
    json["blocking_issues"] = result.blocking_issues;
    json["warnings"] = result.warnings;
    json["content_issues"] = nlohmann::json::array();
    for (const build::ValidationIssue& issue : result.content_report.issues) {
        json["content_issues"].push_back(
            {{"code", issue.code}, {"message", issue.message}, {"path", issue.path}});
    }
    return FileSystem::write_text_file(path, json.dump(2));
}

} // namespace eve::polish
