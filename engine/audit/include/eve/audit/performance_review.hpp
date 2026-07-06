#pragma once

#include <eve/audit/types.hpp>

#include <string_view>

namespace eve::audit {

class PerformanceReviewer {
public:
    std::vector<PerformanceMeasurement> measure_all(std::string_view data_root,
                                                    std::string_view game_root) const;
};

} // namespace eve::audit
