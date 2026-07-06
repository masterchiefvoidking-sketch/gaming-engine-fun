#include <cstdlib>
#include <eve/core/logging/logger.hpp>
#include <eve/example/apartment_character_scene.hpp>
#include <eve/human/character_interaction.hpp>
#include <eve/human/photo_mode_bridge.hpp>
#include <iostream>

int main(int argc, char** argv) {
    eve::Logger::instance().set_level(eve::LogLevel::Info);
    EVE_LOG(Info, "Example", eve::kEngineName, " — Apartment Character Demo");

    const char* data_root = std::getenv("EVE_DATA_DIR");
    if (data_root == nullptr) {
        data_root = "data";
    }
    if (argc > 1) {
        data_root = argv[1];
    }

    eve::example::ApartmentCharacterScene scene;
    if (!scene.initialize(data_root)) {
        std::cerr << "Failed to initialize example scene\n";
        return 1;
    }

    scene.navigate_to_room("living_room");
    scene.character().creator().apply_body_preset(eve::human::BodyTypePreset::Curvy);
    scene.character().creator().set_morph("smile", 0.3f);
    scene.interact_with("sofa", eve::human::CharacterInteractionType::Sit);
    scene.interact_with("tv_living", eve::human::CharacterInteractionType::Look);

    for (int frame = 0; frame < 120; ++frame) {
        scene.update(1.0f / 60.0f);
    }

    scene.character().photo_mode().enable(true);
    eve::human::apply_photo_expression_preset(scene.character().creator(), "smile");
    scene.character().photo_mode().capture_screenshot("example_screenshot.hdr");

    const std::string profile_path = "example_character.json";
    scene.character().creator().save_to_file(profile_path);
    EVE_LOG(Info, "Example", "Saved character profile to ", profile_path);

    scene.shutdown();
    EVE_LOG(Info, "Example", "Demo complete");
    return 0;
}
