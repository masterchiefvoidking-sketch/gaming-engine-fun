#include <eve/product/release_bundle.hpp>

#include <eve/build/build_pipeline.hpp>
#include <eve/build/export_profiles.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/product/bug_triage.hpp>
#include <eve/product/content_completion.hpp>
#include <eve/product/project_archive_enhanced.hpp>
#include <eve/product/release_checklist.hpp>
#include <eve/product/save_stress_tester.hpp>

#include <array>
#include <filesystem>
#include <sstream>

namespace eve::product {

namespace {

void copy_tree(const std::filesystem::path& src, const std::filesystem::path& dest) {
    if (!std::filesystem::exists(src)) {
        return;
    }
    std::filesystem::create_directories(dest);
    std::filesystem::copy(src, dest,
                          std::filesystem::copy_options::recursive |
                              std::filesystem::copy_options::overwrite_existing);
}

void write_text(const std::filesystem::path& path, const std::string& content) {
    FileSystem::create_directories(path.parent_path().string());
    FileSystem::write_text_file(path.string(), content);
}

std::string checklist_md(const ReleaseChecklistReport& report) {
    std::ostringstream out;
    out << "# Validation Report\n\n";
    out << "Passed: " << report.passed_count << "/" << report.items.size() << "\n\n";
    for (const ChecklistItem& item : report.items) {
        out << "- [" << (item.passed ? "x" : " ") << "] " << item.label << "\n";
    }
    return out.str();
}

std::string content_md(const ContentCompletionReport& report) {
    std::ostringstream out;
    out << "# Performance / Content Report\n\n";
    out << "| Metric | Count | Required |\n";
    out << "|--------|-------|----------|\n";
    out << "| Wardrobe items | " << report.wardrobe_items << " | 20 |\n";
    out << "| Hairstyles | " << report.hairstyles << " | 5 |\n";
    out << "| Expressions | " << report.expressions << " | 10 |\n";
    out << "| Rooms | " << report.rooms << " | 6 |\n";
    out << "| Object interactions | " << report.object_interactions << " | 50 |\n";
    out << "| Dialogue lines | " << report.dialogue_lines << " | 100 |\n";
    out << "| Events | " << report.events << " | 10 |\n";
    out << "| Camera presets | " << report.camera_presets << " | 10 |\n";
    out << "| Lighting presets | " << report.lighting_presets << " | 10 |\n";
    out << "| Photo presets | " << report.photo_presets << " | 5 |\n";
    out << "\nStatus: " << (report.complete ? "COMPLETE" : "INCOMPLETE") << "\n";
    return out.str();
}

} // namespace

ReleaseBundleResult ReleaseBundle::create(const ReleaseBundleConfig& config) const {
    ReleaseBundleResult result;
    const std::filesystem::path bundle_root =
        std::filesystem::path(config.output_root) / config.bundle_name;
    result.bundle_root = bundle_root.string();

    FileSystem::create_directories(bundle_root.string());

    const std::array<std::pair<const char*, build::ExportProfile>, 4> profiles = {{
        {"WindowsDev", build::ExportProfile::WindowsDevelopment},
        {"WindowsRelease", build::ExportProfile::WindowsRelease},
        {"WebDev", build::ExportProfile::WebDevelopment},
        {"WebRelease", build::ExportProfile::WebRelease},
    }};

    build::BuildPipeline pipeline;
    for (const auto& [folder, profile] : profiles) {
        const std::filesystem::path dest = bundle_root / folder;
        const std::string build_src =
            config.game_root + "/Build/" + build::profile_name(profile);
        copy_tree(build_src, dest);
        if (!std::filesystem::exists(dest)) {
            build::BuildPipelineConfig build_cfg;
            build_cfg.project_root = config.game_root;
            build_cfg.project_name = "apartment_life";
            build_cfg.profile = profile;
            build_cfg.backup_root = config.game_root + "/Backups";
            const build::BuildPipelineResult built = pipeline.export_project(build_cfg);
            if (built.success) {
                copy_tree(built.output_path, dest);
            }
        }
        result.artifacts.push_back(dest.string());
    }

    ReleaseChecklistConfig checklist_cfg;
    checklist_cfg.data_root = config.data_root;
    checklist_cfg.game_root = config.game_root;
    checklist_cfg.releases_root = config.output_root;
    const ReleaseChecklistReport checklist = ReleaseChecklist().run_all(checklist_cfg);
    write_text(bundle_root / "ValidationReport.md", checklist_md(checklist));

    const ContentCompletionReport content =
        ContentCompletionValidator().validate(config.game_root, config.data_root);
    write_text(bundle_root / "PerformanceReport.md", content_md(content));

    SaveStressConfig stress_cfg;
    stress_cfg.data_root = config.data_root;
    stress_cfg.game_root = config.game_root;
    stress_cfg.slot_count = 10;
    stress_cfg.cycle_count = 100;
    const SaveStressResult stress = SaveStressTester().run(stress_cfg);

    BugTriage triage;
    const BugTriageReport bugs = triage.evaluate();

    std::ostringstream test_report;
    test_report << "# Test Report\n\n";
    test_report << "- Checklist: " << (checklist.passed ? "PASS" : "FAIL") << "\n";
    test_report << "- Content: " << (content.complete ? "PASS" : "FAIL") << "\n";
    test_report << "- Save stress: " << (stress.passed ? "PASS" : "FAIL") << "\n";
    test_report << "- Bug triage: " << (bugs.release_allowed ? "PASS" : "FAIL") << "\n";
    write_text(bundle_root / "TestReport.md", test_report.str());

    std::ostringstream known;
    known << "# Known Issues\n\n";
    known << "Release allowed: " << (bugs.release_allowed ? "yes" : "no") << "\n\n";
    for (const TriagedBug& bug : bugs.bugs) {
        known << "- **" << bug.id << "** (P" << static_cast<int>(bug.priority) << "): "
              << bug.summary;
        if (!bug.workaround.empty()) {
            known << " — workaround: " << bug.workaround;
        }
        known << "\n";
    }
    write_text(bundle_root / "KnownIssues.md", known.str());
    triage.save_known_issues((bundle_root / "known_issues.json").string());

    write_text(bundle_root / "ExportInstructions.md",
               "# Export Instructions\n\n"
               "1. Launch EVE Creator Studio.\n"
               "2. Open Apartment Life Demo.\n"
               "3. Edit character, wardrobe, apartment, dialogue, and events.\n"
               "4. Save project.\n"
               "5. Export Windows and Web builds from the Export panel.\n"
               "6. Artifacts are staged under Releases/ApartmentLifeDemo_RC1/.\n");

    ArchiveOptions archive_opts;
    archive_opts.include_saves = true;
    archive_opts.include_screenshots = true;
    const std::filesystem::path archive_dest = bundle_root / "BackupArchive";
    const build::ArchiveResult archived =
        ProjectArchiveEnhanced().export_archive(config.game_root, archive_dest.string(),
                                                archive_opts);
    if (archived.success) {
        result.artifacts.push_back(archived.archive_path);
    }

    result.success = checklist.passed && content.complete && stress.passed && bugs.release_allowed &&
                     archived.success;
    result.message = result.success ? "Release bundle created" : "Release bundle incomplete";
    return result;
}

} // namespace eve::product
