#include <eve/architecture_freeze/architecture_freeze_app.hpp>

#include <eve/core/logging/logger.hpp>

namespace eve::architecture_freeze {

bool ArchitectureFreezeApp::initialize(std::string_view repo_root, std::string_view data_root,
                                       std::string_view game_root) {
    config_.repo_root = std::string(repo_root);
    config_.data_root = std::string(data_root);
    config_.game_root = std::string(game_root);
    config_.output_dir = config_.repo_root + "/Audit";
    initialized_ = true;
    EVE_LOG(Info, "Audit", "Architecture freeze pipeline ready");
    return true;
}

audit::ArchitectureFreezeReport ArchitectureFreezeApp::run() {
    if (!initialized_) {
        return {};
    }
    report_ = pipeline_.run(config_);
    pipeline_.write_deliverables(config_, report_);
    return report_;
}

} // namespace eve::architecture_freeze
