#include <eve/editor/creator_studio.hpp>
#include <eve/core/logging/logger.hpp>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    const std::string project_path =
        argc > 1 ? argv[1] : "Game/ApartmentLifeDemo";

    eve::Logger::instance().set_level(eve::LogLevel::Info);
    EVE_LOG(Info, "CreatorStudio", "EVE Creator Studio");

    eve::editor::CreatorStudio studio;
    eve::editor::CreatorStudioConfig config;
    config.project_root = project_path;
    config.title = "Apartment Life Demo";
    config.autosave_enabled = true;

    if (!studio.initialize(config)) {
        std::cerr << "Failed to initialize Creator Studio\n";
        return 1;
    }

    if (!studio.open_project(project_path)) {
        std::cerr << "Failed to open project: " << project_path << "\n";
        return 1;
    }

    auto& project = studio.project();
    std::cout << "Project: " << project.layout().title() << "\n";
    std::cout << "Characters: " << project.characters().character_count() << "\n";
    std::cout << "Wardrobe items: " << project.wardrobe().item_count() << "\n";
    std::cout << "Furniture: " << project.apartments().furniture_count() << "\n";
    std::cout << "Dialogue packages: " << project.dialogue().package_count() << "\n";
    std::cout << "Events: " << project.events().event_count() << "\n";
    std::cout << "Localization entries: " << project.localization().entry_count() << "\n";

  const auto search = studio.asset_browser().search("mira");
    std::cout << "Search 'mira': " << search.size() << " results\n";

    studio.export_game(eve::content::ExportTarget::Windows);
    studio.export_game(eve::content::ExportTarget::Linux);
    studio.save_project();

    std::cout << "Creator Studio ready.\n";
    return 0;
}
