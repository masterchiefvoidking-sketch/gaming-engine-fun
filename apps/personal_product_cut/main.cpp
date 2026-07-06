#include <eve/personal_product_cut/personal_product_cut_app.hpp>
#include <eve/core/logging/logger.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    const std::string data_root = argc > 1 ? argv[1] : EVE_DATA_DIR;
    const std::string game_root = argc > 2 ? argv[2] : EVE_GAME_DIR "/ApartmentLifeDemo";
    const std::string releases_root = argc > 3 ? argv[3] : EVE_RELEASES_DIR;

    eve::Logger::instance().set_level(eve::LogLevel::Info);
    EVE_LOG(Info, "ProductCut", "EVE Personal Product Cut — Phase 14");

    eve::personal_product_cut::PersonalProductCutApp app;
    if (!app.initialize(data_root, game_root, releases_root)) {
        std::cerr << "Failed to initialize personal product cut\n";
        return 1;
    }

    const auto report = app.run();
    std::cout << "Content complete: " << (report.content.complete ? "yes" : "no") << "\n";
    std::cout << "Checklist passed: " << (report.checklist.passed ? "yes" : "no") << "\n";
    std::cout << "Save stress passed: " << (report.save_stress.passed ? "yes" : "no") << "\n";
    std::cout << "Bundle success: " << (report.bundle.success ? "yes" : "no") << "\n";
    std::cout << report.message << "\n";
    return report.success ? 0 : 1;
}
