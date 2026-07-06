#pragma once

#include <eve/ai/relationship_simulation.hpp>
#include <eve/interaction/browser_storage.hpp>
#include <eve/interaction/character_inspection_mode.hpp>
#include <eve/interaction/input_controller.hpp>
#include <eve/interaction/interaction_ring.hpp>
#include <eve/interaction/player_ui_state.hpp>
#include <eve/interaction/quality_settings.hpp>
#include <eve/interaction/web_performance_profile.hpp>
#include <eve/platform/game_session.hpp>
#include <eve/platform/unified_save.hpp>

#include <string>

namespace eve::interaction {

struct WebCharacterViewerConfig {
    std::string data_root;
    std::string game_root;
    std::string project_id = "apartment_life";
    std::string character_id = "mira";
    std::string character_name = "Mira";
    std::string romance_subdir = "apartment_life/romance_sim";
    std::string default_outfit = "mira_outfit_01";
    std::string default_hair = "mira_hair_bun";
    std::string default_expression = "soft_smile";
    std::string default_greet_dialogue = "greet_evening";
    std::string initial_room = "living_room";
    std::string save_slot = "viewer_slot";
    StorageBackend storage_backend = StorageBackend::LocalFile;
    f32 viewport_width = 1920.0f;
    f32 viewport_height = 1080.0f;
    bool offline_only = true;
};

class WebCharacterViewer {
public:
    bool initialize(const WebCharacterViewerConfig& config);
    void shutdown();
    void update(f32 delta_seconds);

    bool handle_select();
    bool handle_focus();
    bool open_interaction_ring();
    bool execute_ring_action(RingActionType action);
    bool change_outfit(std::string_view outfit_id);
    bool change_hair(std::string_view hair_id);
    bool change_expression(std::string_view expression_id);
    bool enter_photo_mode(bool enabled);
    bool capture_photo(std::string_view path);
    bool save_session();
    bool load_session();
    bool publish_web(std::string_view output_dir);

    void set_quality(const QualitySettings& settings);
    void apply_web_profile();
    void apply_mobile_profile();

    [[nodiscard]] InputController& input() { return input_; }
    [[nodiscard]] CharacterInspectionMode& inspection() { return inspection_; }
    [[nodiscard]] InteractionRing& ring() { return ring_; }
    [[nodiscard]] PlayerUIState& ui() { return ui_; }
    [[nodiscard]] const std::vector<InteractionRingAction>& ring_actions() const {
        return ring_actions_;
    }
    [[nodiscard]] ai::RelationshipSimulation& relationship() { return relationship_; }

private:
    void refresh_ring_context();
    human::InteractionContextSnapshot build_context() const;

    WebCharacterViewerConfig config_;
    InputController input_;
    CharacterInspectionMode inspection_;
    InteractionRing ring_;
    PlayerUIState ui_;
    WebPerformanceProfile performance_;
    platform::UnifiedSaveSystem save_system_;
    platform::UnifiedSaveGame save_data_;
    SaveStorageRouter storage_;
    platform::WebPublisher web_publisher_;
    ai::RelationshipSimulation relationship_;
    std::vector<InteractionRingAction> ring_actions_;
    InteractionRingContext ring_context_;
    bool initialized_ = false;
    bool ring_open_ = false;
};

} // namespace eve::interaction
