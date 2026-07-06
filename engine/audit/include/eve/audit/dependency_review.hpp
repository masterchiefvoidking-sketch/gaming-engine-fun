#pragma once

#include <eve/audit/types.hpp>

namespace eve::audit {

class DependencyReviewer {
public:
    std::vector<DependencyEntry> audit() const;
};

} // namespace eve::audit
