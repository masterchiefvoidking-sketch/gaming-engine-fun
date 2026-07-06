#pragma once

#include <eve/ai/relationship_simulation.hpp>
#include <eve/build/build_pipeline.hpp>
#include <eve/content/game_content_project.hpp>
#include <eve/interaction/browser_storage.hpp>
#include <eve/interaction/web_character_viewer.hpp>
#include <eve/platform/unified_save.hpp>
#include <eve/scene/scene_director.hpp>

#include <eve/animation/anime_motion.hpp>
#include <eve/human/anime_expression_system.hpp>
#include <eve/world/anime_room_lighting.hpp>

#include <eve/polish/animation_polish.hpp>
#include <eve/polish/apartment_atmosphere.hpp>
#include <eve/polish/camera_polish.hpp>
#include <eve/polish/dialogue_polish.hpp>
#include <eve/polish/graphics_polish.hpp>
#include <eve/polish/performance_profiler.hpp>
#include <eve/polish/validation_gate.hpp>
#include <eve/polish/wardrobe_polish.hpp>

#include <string>
#include <vector>

namespace eve::gameplay {

struct ApartmentLifeConfig {
    std::string data_root;
    std::string game_root;
    std::string character_id = "mira";
    std::string character_name = "Mira";
    std::string project_id = "apartment_life";
    std::string romance_subdir = "apartment_life/romance_sim";
    std::string save_slot = "slot_01";
    std::string apartment_id = "sunset_apartment";
    std::string default_outfit = "mira_outfit_01";
    std::string default_hair = "mira_hair_bun";
    std::string default_expression = "soft_smile";
    std::string initial_room = "living_room";
    interaction::StorageBackend storage_backend = interaction::StorageBackend::LocalFile;
    f32 viewport_width = 1920.0f;
    f32 viewport_height = 1080.0f;
    bool offline_only = true;
};

struct RoomInteraction {
    std::string room_id;
    std::string interaction_id;
    std::string label;
};

struct EventPlayResult {
    bool success = false;
    std::string event_id;
    std::string dialogue_line;
    std::string rejection_reason;
    std::string memory_id;
};

struct VerticalSliceStep {
    std::string phase;
    bool success = false;
    std::string detail;
};

class ApartmentLifeSession {
public:
    bool initialize(const ApartmentLifeConfig& config);
    void shutdown();
    void update(f32 delta_seconds);

    bool change_room(std::string_view room_id);
    [[nodiscard]] std::string current_room() const;
    [[nodiscard]] const std::vector<std::string>& room_ids() const { return room_ids_; }

    bool interact(std::string_view interaction_id);
    EventPlayResult trigger_event(std::string_view event_id);
    bool talk(std::string_view dialogue_node_id);
    bool make_dialogue_choice(std::string_view node_id, std::string_view choice_id);

    bool change_outfit(std::string_view outfit_id);
    bool change_hair(std::string_view hair_id);
    bool change_expression(std::string_view expression_id);
    bool randomize_outfit();
    bool enter_photo_mode(bool enabled);
    bool capture_photo(std::string_view path);

    bool save_game();
    bool load_game();
    bool auto_save();

    bool validate_content() const;
    bool export_windows_dev(std::string& output_path);
    bool export_web_dev(std::string& output_path);

    std::vector<VerticalSliceStep> run_vertical_slice();

    [[nodiscard]] content::GameContentProject& content() { return content_; }
    [[nodiscard]] ai::RelationshipSimulation& relationship() { return relationship_; }
    [[nodiscard]] scene::SceneDirector& director() { return director_; }
    [[nodiscard]] interaction::WebCharacterViewer& viewer() { return viewer_; }
    [[nodiscard]] polish::PerformanceProfiler& profiler() { return profiler_; }
    [[nodiscard]] polish::GraphicsPolish& graphics() { return graphics_; }
    [[nodiscard]] polish::CameraPolish& camera_polish() { return camera_polish_; }
    [[nodiscard]] polish::WardrobePolish& wardrobe_polish() { return wardrobe_polish_; }
    [[nodiscard]] polish::ApartmentAtmosphere& atmosphere() { return atmosphere_; }
    [[nodiscard]] const ApartmentLifeConfig& config() const { return config_; }
    [[nodiscard]] bool initialized() const { return initialized_; }

private:
    void sync_character_state();
    void refresh_room_interactions();
    interaction::WebCharacterViewerConfig build_viewer_config() const;
    std::string save_path() const;

    ApartmentLifeConfig config_;
    content::GameContentProject content_;
    ai::RelationshipSimulation relationship_;
    scene::SceneDirector director_;
    interaction::WebCharacterViewer viewer_;
    platform::UnifiedSaveSystem save_system_;
    platform::UnifiedSaveGame save_data_;
    world::AnimeRoomLightingSystem lighting_;
    animation::AnimeMotionController motion_;
    human::AnimeExpressionSystem expressions_;
    polish::PerformanceProfiler profiler_;
    polish::GraphicsPolish graphics_;
    polish::CameraPolish camera_polish_;
    polish::AnimationPolish animation_polish_;
    polish::WardrobePolish wardrobe_polish_;
    polish::ApartmentAtmosphere atmosphere_;
    polish::DialoguePolish dialogue_polish_;
    std::vector<std::string> room_ids_;
    std::vector<RoomInteraction> room_interactions_;
    std::string current_room_;
    bool scene_active_ = false;
    bool initialized_ = false;
};

} // namespace eve::gameplay
