#include <eve/apartment_demo/apartment_life_demo_app.hpp>

#include <eve/core/logging/logger.hpp>

namespace eve::apartment_demo {

bool ApartmentLifeDemoApp::initialize(std::string_view data_root, std::string_view game_root) {
    gameplay::ApartmentLifeConfig config;
    config.data_root = std::string(data_root);
    config.game_root = std::string(game_root);
    config.character_id = "mira";
    config.character_name = "Mira";
    config.project_id = "apartment_life";
    config.save_slot = "apartment_life_slot";
    config.offline_only = true;
    config.viewport_width = 1280.0f;
    config.viewport_height = 720.0f;
    if (!session_.initialize(config)) {
        return false;
    }
    initialized_ = true;
    EVE_LOG(Info, "ApartmentLifeDemo", "Apartment Life Demo ready");
    return true;
}

void ApartmentLifeDemoApp::shutdown() {
    if (initialized_) {
        session_.shutdown();
        initialized_ = false;
    }
}

void ApartmentLifeDemoApp::update(f32 delta_seconds) {
    if (initialized_) {
        session_.update(delta_seconds);
    }
}

bool ApartmentLifeDemoApp::run_vertical_slice() {
    if (!initialized_) {
        return false;
    }
    last_run_ = session_.run_vertical_slice();
    for (const gameplay::VerticalSliceStep& step : last_run_) {
        if (!step.success && (step.phase == "init" || step.phase == "save" || step.phase == "load" ||
                              step.phase == "validate_content")) {
            EVE_LOG(Warn, "ApartmentLifeDemo", "Slice step failed: ", step.phase, " - ", step.detail);
            return false;
        }
    }
    return true;
}

} // namespace eve::apartment_demo
