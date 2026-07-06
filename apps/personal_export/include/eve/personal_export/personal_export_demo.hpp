#pragma once

#include <eve/build/build_pipeline.hpp>

#include <string>

namespace eve::personal_export {

struct PersonalExportDemoResult {
    bool success = false;
    std::string windows_output;
    std::string web_output;
    std::string message;
};

class PersonalExportDemo {
public:
    PersonalExportDemoResult run(std::string_view project_root, std::string_view project_name);
};

} // namespace eve::personal_export
