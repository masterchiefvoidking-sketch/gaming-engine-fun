#include <eve/personal_export/personal_export_demo.hpp>

#include <eve/core/logging/logger.hpp>

#include <cstdlib>
#include <iostream>

int main() {
    constexpr const char* kProject = EVE_GAME_DIR "/ApartmentLifeDemo";
    eve::personal_export::PersonalExportDemo demo;
    const eve::personal_export::PersonalExportDemoResult result =
        demo.run(kProject, "Apartment Life Demo");
    if (!result.success) {
        EVE_LOG(Error, "PersonalExport", result.message);
        return EXIT_FAILURE;
    }
    std::cout << "Windows: " << result.windows_output << "\n";
    std::cout << "Web: " << result.web_output << "\n";
    return EXIT_SUCCESS;
}
