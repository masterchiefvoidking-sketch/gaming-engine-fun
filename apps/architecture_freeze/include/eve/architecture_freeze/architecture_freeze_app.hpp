#pragma once

#include <eve/audit/architecture_freeze.hpp>

#include <string>

namespace eve::architecture_freeze {

class ArchitectureFreezeApp {
public:
    bool initialize(std::string_view repo_root, std::string_view data_root,
                    std::string_view game_root);
    audit::ArchitectureFreezeReport run();
    [[nodiscard]] const audit::ArchitectureFreezeReport& last_report() const { return report_; }

private:
    audit::ArchitectureFreezeConfig config_;
    audit::ArchitectureFreeze pipeline_;
    audit::ArchitectureFreezeReport report_;
    bool initialized_ = false;
};

} // namespace eve::architecture_freeze
