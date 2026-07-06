#pragma once

#include <eve/audit/types.hpp>

#include <string_view>

namespace eve::audit {

class TestCoverageAuditor {
public:
    std::vector<TestCoverageEntry> audit(std::string_view repo_root) const;
};

} // namespace eve::audit
