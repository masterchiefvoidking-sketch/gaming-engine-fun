#pragma once

#include <eve/audit/types.hpp>

#include <string_view>

namespace eve::audit {

class MemoryReviewer {
public:
    std::vector<MemoryCategoryReport> audit(std::string_view data_root,
                                            std::string_view game_root) const;
};

} // namespace eve::audit
