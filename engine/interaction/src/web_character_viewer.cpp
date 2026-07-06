#include <eve/interaction/web_character_viewer.hpp>

#include <eve/ai/character_sim_state.hpp>
#include <eve/core/logging/logger.hpp>

namespace eve::interaction {

bool WebCharacterViewer::initialize(const WebCharacterViewerConfig& config) {
    config_ = config;
    if (!inspection_.initialize(config_.data_root)) {
        return false;
    }
    if (!relationship_.initialize(config_.data_root)) {
        return false;
    }
    storage_.set_desktop_storage(LocalFileStorage(config_.data_root + "/saves"));
    storage_.set_browser_storage(BrowserIndexedDbStorage(config_.data_root + "/browser_cache"));
    storage_.set_active_backend(config_.storage_backend);
    ui_.layout().set_viewport(config_.viewport_width, config_.viewport_height);
    performance_.apply_web_defaults();
    save_data_.project_id = config_.project_id;
    save_data_.save_slot = config_.save_slot;
    ring_context_.is_character = true;
    ring_context_.target_id = "mira";
    ring_context_.target_label = "Mira";
    ring_context_.interaction.room_id = "living_room";
    refresh_ring_context();
    initialized_ = true;
    EVE_LOG(Info, "WebCharacterViewer", "Viewer ready (offline=", config_.offline_only, ")");
    return true;
}

void WebCharacterViewer::shutdown() {
    if (initialized_) {
        inspection_.shutdown();
        initialized_ = false;
    }
}

void WebCharacterViewer::update(f32 delta_seconds) {
    if (!initialized_) {
        return;
    }
    inspection_.update(delta_seconds, input_);
}

human::InteractionContextSnapshot WebCharacterViewer::build_context() const {
    human::InteractionContextSnapshot context;
    context.room_id = ring_context_.interaction.room_id;
    context.outfit_id = inspection_.preview().outfit_id;
    context.mood = "neutral";
    const ai::CharacterSimState* character = relationship_.character();
    if (character != nullptr) {
        context.trust = character->trust;
        context.affection = character->affection;
        context.relationship = character->affection;
        context.mood = character->mood == ai::Mood::Happy ? "happy" : "neutral";
    }
    context.privacy_level = 1.0f;
    context.player_permission = true;
    return context;
}

void WebCharacterViewer::refresh_ring_context() {
    ring_context_.interaction = build_context();
    ring_actions_ = ring_.build_actions(ring_context_);
}

bool WebCharacterViewer::handle_select() {
    const human::BodyRegionTarget target =
        inspection_.raycast_body_region({0.0f, 1.5f, 2.5f}, {0.0f, 0.0f, -1.0f});
    if (target.hovered) {
        inspection_.focus_body_region(target.region);
        ring_context_.body_region = target.region;
        ring_context_.is_character = true;
        refresh_ring_context();
        return true;
    }
    return false;
}

bool WebCharacterViewer::handle_focus() {
    return inspection_.focus_body_region(ring_context_.body_region);
}

bool WebCharacterViewer::open_interaction_ring() {
    ring_open_ = true;
    refresh_ring_context();
    return !ring_actions_.empty();
}

bool WebCharacterViewer::execute_ring_action(RingActionType action) {
    refresh_ring_context();
    const InteractionRingAction selected = ring_.action_for_type(action, ring_context_);
    if (!selected.enabled && action != RingActionType::Cancel) {
        return false;
    }

    switch (action) {
    case RingActionType::Look:
        return handle_focus();
    case RingActionType::Talk:
        relationship_.talk("greet_evening");
        refresh_ring_context();
        return true;
    case RingActionType::Compliment:
        relationship_.offer_compliment();
        refresh_ring_context();
        return true;
    case RingActionType::ChangeOutfit:
        ui_.set_panel_visible(UIPanel::Wardrobe, true);
        return true;
    case RingActionType::Pose:
        return inspection_.preview_pose("full_body_wardrobe");
    case RingActionType::Inspect:
        return handle_select();
    case RingActionType::Cancel:
        ring_open_ = false;
        return true;
    case RingActionType::UseObject:
        return false;
    }
    return false;
}

bool WebCharacterViewer::change_outfit(std::string_view outfit_id) {
  if (!inspection_.preview_outfit(outfit_id)) {
        return false;
    }
    refresh_ring_context();
    return true;
}

bool WebCharacterViewer::change_hair(std::string_view hair_id) {
    return inspection_.preview_hair(hair_id);
}

bool WebCharacterViewer::change_expression(std::string_view expression_id) {
    return inspection_.preview_expression(expression_id);
}

bool WebCharacterViewer::enter_photo_mode(bool enabled) {
    inspection_.photo_mode().enable(enabled);
    ui_.set_panel_visible(UIPanel::PhotoMode, enabled);
    return true;
}

bool WebCharacterViewer::capture_photo(std::string_view path) {
    return inspection_.photo_mode().capture(path);
}

bool WebCharacterViewer::save_session() {
    save_data_ = save_system_.capture_from_relationship_sim(relationship_, config_.project_id);
    save_data_.character.outfit = inspection_.preview().outfit_id;
    save_data_.wardrobe.active_outfit_id = inspection_.preview().outfit_id;
    save_data_.settings = {1.0f, 0.7f, 1.0f, "en", static_cast<u32>(performance_.quality.texture_quality),
                           config_.storage_backend == StorageBackend::BrowserIndexedDb};
    return storage_.save_game(save_system_, save_data_, config_.save_slot);
}

bool WebCharacterViewer::load_session() {
    if (!storage_.load_game(save_system_, save_data_, config_.save_slot)) {
        return false;
    }
    save_system_.apply_to_relationship_sim(relationship_, save_data_);
    if (!save_data_.wardrobe.active_outfit_id.empty()) {
        inspection_.preview_outfit(save_data_.wardrobe.active_outfit_id);
    }
    refresh_ring_context();
    return true;
}

bool WebCharacterViewer::publish_web(std::string_view output_dir) {
    platform::WebDeployConfig deploy;
    deploy.project_name = config_.project_id;
    deploy.output_dir = std::string(output_dir);
    deploy.mobile_friendly_controls = true;
    const platform::WebDeployResult result = web_publisher_.publish(deploy, config_.game_root);
    return result.success;
}

void WebCharacterViewer::set_quality(const QualitySettings& settings) {
    performance_.quality = settings;
    performance_.sync_from_quality();
}

void WebCharacterViewer::apply_web_profile() {
    performance_.apply_web_defaults();
}

void WebCharacterViewer::apply_mobile_profile() {
    performance_.apply_mobile_defaults();
}

} // namespace eve::interaction
