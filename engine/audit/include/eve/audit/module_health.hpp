#pragma once

#include <eve/audit/types.hpp>

#include <string_view>

namespace eve::audit {

class ModuleHealthAuditor {
public:
    std::vector<ModuleHealthReport> audit_all(std::string_view repo_root) const;
};

} // namespace eve::audit
