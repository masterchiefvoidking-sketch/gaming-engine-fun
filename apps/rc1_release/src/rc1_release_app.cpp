#include <eve/rc1_release/rc1_release_app.hpp>

#include <eve/core/logging/logger.hpp>

namespace eve::rc1_release {

bool RC1ReleaseApp::initialize(std::string_view data_root, std::string_view game_root) {
    config_.data_root = std::string(data_root);
    config_.game_root = std::string(game_root);
    config_.output_root = config_.game_root + "/Build/rc1";
    config_.project_name = "apartment_life";
    initialized_ = true;
    EVE_LOG(Info, "RC1Release", "RC1 release pipeline ready");
    return true;
}

polish::RC1ReleaseReport RC1ReleaseApp::run() {
    if (!initialized_) {
        return {};
    }
    last_report_ = pipeline_.build_all(config_);
    return last_report_;
}

} // namespace eve::rc1_release
