#pragma once

#include <eve/build/export_profiles.hpp>

#include <string>
#include <vector>

namespace eve::build {

enum class ValidationSeverity : u8 {
    Info = 0,
    Warning,
    Error,
};

struct ValidationIssue {
    ValidationSeverity severity = ValidationSeverity::Error;
    std::string code;
    std::string message;
    std::string path;
};

struct ValidationReport {
    bool passed = false;
    std::vector<ValidationIssue> issues;

    void add_error(std::string code, std::string message, std::string path = {});
    void add_warning(std::string code, std::string message, std::string path = {});
    [[nodiscard]] bool has_errors() const;
};

class ContentValidator {
public:
    ValidationReport validate(std::string_view project_root,
                              const ExportProfileSettings& profile) const;
};

} // namespace eve::build
