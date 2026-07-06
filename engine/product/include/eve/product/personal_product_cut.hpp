#pragma once

#include <eve/product/bug_triage.hpp>
#include <eve/product/content_completion.hpp>
#include <eve/product/project_archive_enhanced.hpp>
#include <eve/product/release_bundle.hpp>
#include <eve/product/release_checklist.hpp>
#include <eve/product/save_stress_tester.hpp>

#include <string>

namespace eve::product {

struct PersonalProductConfig {
    std::string data_root;
    std::string game_root;
    std::string releases_root;
};

struct PersonalProductReport {
    bool success = false;
    ContentCompletionReport content;
    ReleaseChecklistReport checklist;
    SaveStressResult save_stress;
    BugTriageReport bug_triage;
    ReleaseBundleResult bundle;
    std::string message;
};

class PersonalProductCut {
public:
    PersonalProductReport run_full_cut(const PersonalProductConfig& config);
    bool run_editor_workflow(std::string_view game_root, std::string_view data_root);
};

} // namespace eve::product
