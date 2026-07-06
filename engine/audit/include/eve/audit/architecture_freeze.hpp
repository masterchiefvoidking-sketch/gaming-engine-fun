#pragma once

#include <eve/audit/types.hpp>

namespace eve::audit {

class ArchitectureFreeze {
public:
    ArchitectureFreezeReport run(const ArchitectureFreezeConfig& config) const;
    bool write_deliverables(const ArchitectureFreezeConfig& config,
                            const ArchitectureFreezeReport& report) const;
};

} // namespace eve::audit
