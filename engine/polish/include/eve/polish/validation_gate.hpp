#pragma once

#include <eve/build/content_validator.hpp>
#include <eve/build/export_profiles.hpp>

#include <string>

namespace eve::polish {

struct ValidationGateResult {
    bool passed = false;
    build::ValidationReport content_report;
    std::vector<std::string> blocking_issues;
    std::vector<std::string> warnings;
};

class ValidationGate {
public:
    ValidationGateResult validate_for_export(std::string_view project_root,
                                             build::ExportProfile profile) const;
    bool check_save_schema(std::string_view save_path) const;
    bool check_wardrobe_compatibility(std::string_view game_root) const;
    bool check_dialogue_nodes(std::string_view game_root) const;
    bool check_event_conditions(std::string_view game_root) const;
    bool check_clipping_severity(std::string_view game_root) const;
    bool save_report(std::string_view path, const ValidationGateResult& result) const;
};

} // namespace eve::polish
