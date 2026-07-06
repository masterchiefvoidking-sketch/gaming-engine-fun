#pragma once

#include <eve/audit/types.hpp>

namespace eve::audit {

class ApiReviewer {
public:
    std::vector<ApiReviewEntry> review() const;
};

} // namespace eve::audit
