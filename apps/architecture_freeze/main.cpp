#include <eve/architecture_freeze/architecture_freeze_app.hpp>
#include <eve/core/logging/logger.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    const std::string repo_root = argc > 1 ? argv[1] : EVE_REPO_DIR;
    const std::string data_root = argc > 2 ? argv[2] : EVE_DATA_DIR;
    const std::string game_root = argc > 3 ? argv[3] : EVE_GAME_DIR "/ApartmentLifeDemo";

    eve::Logger::instance().set_level(eve::LogLevel::Info);
    EVE_LOG(Info, "Audit", "EVE Architecture Freeze — Phase 15");

    eve::architecture_freeze::ArchitectureFreezeApp app;
    if (!app.initialize(repo_root, data_root, game_root)) {
        std::cerr << "Failed to initialize architecture freeze\n";
        return 1;
    }

    const auto report = app.run();
    std::cout << "Modules audited: " << report.modules.size() << "\n";
    std::cout << "Performance measurements: " << report.performance.size() << "\n";
    std::cout << "Outstanding issues: " << report.outstanding_issues.size() << "\n";
    std::cout << report.message << "\n";
    std::cout << "Deliverables written to Audit/\n";
    return report.success ? 0 : 1;
}
