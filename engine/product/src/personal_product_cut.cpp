#include <eve/product/personal_product_cut.hpp>

#include <eve/content/game_content_project.hpp>
#include <eve/build/export_profiles.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/core/logging/logger.hpp>
#include <eve/editor/creator_studio.hpp>

namespace eve::product {

PersonalProductReport PersonalProductCut::run_full_cut(const PersonalProductConfig& config) {
    PersonalProductReport report;

    report.content =
        ContentCompletionValidator().validate(config.game_root, config.data_root);
    report.checklist = ReleaseChecklist().run_all(
        {config.data_root, config.game_root, config.releases_root});

    SaveStressConfig stress_cfg;
    stress_cfg.data_root = config.data_root;
    stress_cfg.game_root = config.game_root;
    stress_cfg.slot_count = 10;
    stress_cfg.cycle_count = 100;
    report.save_stress = SaveStressTester().run(stress_cfg);

    BugTriage triage;
    report.bug_triage = triage.evaluate();

    ReleaseBundleConfig bundle_cfg;
    bundle_cfg.game_root = config.game_root;
    bundle_cfg.data_root = config.data_root;
    bundle_cfg.output_root = config.releases_root;
    report.bundle = ReleaseBundle().create(bundle_cfg);

    report.success = report.content.complete && report.checklist.passed &&
                     report.save_stress.passed && report.bug_triage.release_allowed &&
                     report.bundle.success;
    report.message = report.success ? "Personal product cut complete"
                                    : "Personal product cut has blocking issues";
    EVE_LOG(Info, "ProductCut", report.message);
    return report;
}

bool PersonalProductCut::run_editor_workflow(std::string_view game_root,
                                             std::string_view data_root) {
    editor::CreatorStudio studio;
    editor::CreatorStudioConfig studio_cfg;
    studio_cfg.project_root = std::string(game_root);
    studio_cfg.title = "Apartment Life Demo";
    studio_cfg.autosave_enabled = true;
    if (!studio.initialize(studio_cfg)) {
        return false;
    }
    if (!studio.open_project(game_root)) {
        return false;
    }

    studio.character_editor().set_project(studio.project());
    studio.wardrobe_editor().set_database(studio.project().wardrobe());
    studio.dialogue_editor().set_library(studio.project().dialogue());
    studio.event_editor().set_library(studio.project().events());
    studio.export_panel().set_project(game_root, "Apartment Life Demo");

    if (!studio.export_panel().validate()) {
        EVE_LOG(Warn, "ProductCut", "Export validation reported issues");
    }
    studio.export_game(eve::content::ExportTarget::Windows);
    studio.export_with_profile(build::ExportProfile::WebDevelopment);
    studio.export_with_profile(build::ExportProfile::WebRelease);
    studio.save_project();

  ArchiveOptions opts;
    opts.include_saves = true;
    opts.include_screenshots = true;
    const std::string archive_path = std::string(game_root) + "/Backups/product_cut_archive";
    const build::ArchiveResult archived =
        ProjectArchiveEnhanced().export_archive(game_root, archive_path, opts);

    (void)data_root;
    return archived.success;
}

} // namespace eve::product
