#include <eve/personal_product_cut/personal_product_cut_app.hpp>

#include <eve/core/logging/logger.hpp>

namespace eve::personal_product_cut {

bool PersonalProductCutApp::initialize(std::string_view data_root, std::string_view game_root,
                                       std::string_view releases_root) {
    config_.data_root = std::string(data_root);
    config_.game_root = std::string(game_root);
    config_.releases_root = std::string(releases_root);
    initialized_ = true;
    EVE_LOG(Info, "ProductCut", "Personal product cut pipeline ready");
    return true;
}

product::PersonalProductReport PersonalProductCutApp::run() {
    if (!initialized_) {
        return {};
    }
    pipeline_.run_editor_workflow(config_.game_root, config_.data_root);
    last_report_ = pipeline_.run_full_cut(config_);
    return last_report_;
}

} // namespace eve::personal_product_cut
