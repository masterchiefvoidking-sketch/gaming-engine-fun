#include <eve/gameplay/apartment_life_session.hpp>

#include <cstdio>

#include <eve/build/content_validator.hpp>
#include <eve/core/logging/logger.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <random>

namespace eve::gameplay {

namespace {

constexpr std::array kDemoEvents = {
    "morning_greeting",   "wardrobe_outfit_reveal", "coffee_together", "shared_dinner",
    "rainy_movie_night",  "living_room_music",      "window_conversation",
    "bedroom_goodnight",  "bathroom_grooming",      "apology_comfort",
};

constexpr std::array kDemoRooms = {"living_room", "kitchen",     "bedroom",
                                   "bathroom",    "dining_room", "wardrobe"};

} // namespace

interaction::WebCharacterViewerConfig ApartmentLifeSession::build_viewer_config() const {
    interaction::WebCharacterViewerConfig viewer_config;
    viewer_config.data_root = config_.data_root;
    viewer_config.game_root = config_.game_root;
    viewer_config.project_id = config_.project_id;
    viewer_config.save_slot = config_.save_slot;
    viewer_config.storage_backend = config_.storage_backend;
    viewer_config.viewport_width = config_.viewport_width;
    viewer_config.viewport_height = config_.viewport_height;
    viewer_config.offline_only = config_.offline_only;
    viewer_config.character_id = config_.character_id;
    viewer_config.character_name = config_.character_name;
    viewer_config.romance_subdir = config_.romance_subdir;
    viewer_config.default_outfit = config_.default_outfit;
    viewer_config.default_hair = config_.default_hair;
    viewer_config.default_expression = config_.default_expression;
    viewer_config.initial_room = config_.initial_room;
    return viewer_config;
}

std::string ApartmentLifeSession::save_path() const {
    return config_.data_root + "/saves/" + config_.save_slot + ".json";
}

bool ApartmentLifeSession::initialize(const ApartmentLifeConfig& config) {
    config_ = config;
    if (!content_.open(config_.game_root)) {
        EVE_LOG(Error, "ApartmentLife", "Failed to open game content at ", config_.game_root);
        return false;
    }

    if (!relationship_.initialize(config_.data_root, config_.character_id, config_.romance_subdir)) {
        EVE_LOG(Error, "ApartmentLife", "Failed to initialize relationship simulation");
        return false;
    }

    if (!viewer_.initialize(build_viewer_config())) {
        EVE_LOG(Error, "ApartmentLife", "Failed to initialize character viewer");
        return false;
    }

    viewer_.apply_web_profile();

    if (!director_.initialize(config_.data_root, relationship_)) {
        EVE_LOG(Error, "ApartmentLife", "Failed to initialize scene director");
        return false;
    }

    director_.attach_camera(viewer_.inspection().camera());
    lighting_.load_presets(config_.data_root + "/anime/room_lighting.json");
    expressions_.load_presets(config_.data_root + "/anime/expressions.json");
    director_.attach_lighting(lighting_);
    director_.attach_motion(motion_);
    director_.attach_expressions(expressions_);

    graphics_.apply_preset(polish::GraphicsPreset::High);
    interaction::QualitySettings quality;
    graphics_.apply_quality_tier(quality);
    viewer_.set_quality(quality);
    graphics_.apply_to_material(viewer_.inspection().pipeline().material());
    graphics_.apply_to_outline(viewer_.inspection().pipeline().outline());
    camera_polish_.load_room_presets(config_.data_root + "/camera/room_presets.json");
    camera_polish_.set_active_room(config_.initial_room);
    animation_polish_.attach(motion_, expressions_, viewer_.inspection().pipeline().eyes());
    wardrobe_polish_.initialize(config_.game_root);
    atmosphere_.load(config_.data_root);
    atmosphere_.apply_time_preset(lighting_, polish::TimeOfDayPreset::Sunset);
    atmosphere_.apply_room(current_room_, lighting_);

    profiler_.set_budget({60.0f, 16.67f, 2048.0f, 5000});
    profiler_.set_frame_cap(120);

    const content::ApartmentContentDefinition* apartment =
        content_.apartments().find_apartment(config_.apartment_id);
    if (apartment != nullptr) {
        room_ids_ = apartment->room_ids;
    } else {
        room_ids_ = {kDemoRooms.begin(), kDemoRooms.end()};
    }

    current_room_ = config_.initial_room;
    sync_character_state();
    refresh_room_interactions();

    save_data_.project_id = config_.project_id;
    save_data_.save_slot = config_.save_slot;
    save_data_.apartment.apartment_id = config_.apartment_id;
    save_data_.apartment.active_room = current_room_;

    initialized_ = true;
    EVE_LOG(Info, "ApartmentLife", "Apartment Life session ready for ", config_.character_name);
    return true;
}

void ApartmentLifeSession::shutdown() {
    if (initialized_) {
        viewer_.shutdown();
        scene_active_ = false;
        initialized_ = false;
    }
}

void ApartmentLifeSession::sync_character_state() {
    ai::CharacterSimState* character = relationship_.character();
    if (character == nullptr) {
        return;
    }
    character->current_room_id = current_room_;
    character->current_outfit_id = config_.default_outfit;
    viewer_.change_outfit(character->current_outfit_id);
    viewer_.change_hair(config_.default_hair);
    viewer_.change_expression(config_.default_expression);
}

void ApartmentLifeSession::refresh_room_interactions() {
    room_interactions_.clear();
    if (current_room_ == "bedroom") {
        room_interactions_.push_back({current_room_, "sit", "Sit"});
        room_interactions_.push_back({current_room_, "sleep", "Sleep"});
        room_interactions_.push_back({current_room_, "read", "Read"});
        room_interactions_.push_back({current_room_, "change_outfit", "Change outfit"});
    } else if (current_room_ == "bathroom") {
        room_interactions_.push_back({current_room_, "mirror", "Mirror inspection"});
        room_interactions_.push_back({current_room_, "grooming", "Grooming routine"});
    } else if (current_room_ == "living_room") {
        room_interactions_.push_back({current_room_, "sit_sofa", "Sit on sofa"});
        room_interactions_.push_back({current_room_, "watch_tv", "Watch TV"});
        room_interactions_.push_back({current_room_, "listen_music", "Listen to music"});
        room_interactions_.push_back({current_room_, "photo_pose", "Photo pose"});
    } else if (current_room_ == "kitchen" || current_room_ == "dining_room") {
        room_interactions_.push_back({current_room_, "prepare_food", "Prepare food"});
        room_interactions_.push_back({current_room_, "eat_together", "Eat together"});
        room_interactions_.push_back({current_room_, "drink_tea", "Drink tea"});
    } else if (current_room_ == "wardrobe") {
        room_interactions_.push_back({current_room_, "change_outfit", "Change outfit"});
        room_interactions_.push_back({current_room_, "mirror_preview", "Mirror preview"});
        room_interactions_.push_back({current_room_, "save_outfit", "Save outfit"});
    }
}

void ApartmentLifeSession::update(f32 delta_seconds) {
    if (!initialized_) {
        return;
    }
    profiler_.begin_frame();
    camera_polish_.apply_smooth_input(viewer_.inspection().camera(), delta_seconds,
                                      viewer_.input().frame());
    animation_polish_.update(delta_seconds);
    viewer_.update(delta_seconds);
    if (scene_active_) {
        const scene::SceneStepResult step = director_.advance(delta_seconds);
        if (step.completed) {
            scene_active_ = false;
        }
    }
    profiler_.record_animation_cost(0.8f);
    profiler_.record_draw_calls(1200);
    profiler_.end_frame(delta_seconds);
}

bool ApartmentLifeSession::change_room(std::string_view room_id) {
    const auto found = std::find(room_ids_.begin(), room_ids_.end(), room_id);
    if (found == room_ids_.end()) {
        return false;
    }
    current_room_ = std::string(room_id);
    ai::CharacterSimState* character = relationship_.character();
    if (character != nullptr) {
        character->current_room_id = current_room_;
    }
    camera_polish_.set_active_room(current_room_);
    atmosphere_.apply_room(current_room_, lighting_);
    refresh_room_interactions();
    return true;
}

std::string ApartmentLifeSession::current_room() const {
    return current_room_;
}

bool ApartmentLifeSession::interact(std::string_view interaction_id) {
    const scene::SceneStartResult started =
        director_.start_from_room(current_room_, interaction_id);
    if (started.success) {
        scene_active_ = true;
        return true;
    }
    if (interaction_id == "change_outfit") {
        viewer_.ui().set_panel_visible(interaction::UIPanel::Wardrobe, true);
        return true;
    }
    if (interaction_id == "photo_pose") {
        return viewer_.enter_photo_mode(true);
    }
    if (interaction_id == "mirror" || interaction_id == "mirror_preview") {
        return viewer_.inspection().preview_pose("mirror_front");
    }
    return talk("room_" + current_room_ + "_talk");
}

EventPlayResult ApartmentLifeSession::trigger_event(std::string_view event_id) {
    EventPlayResult result;
    result.event_id = std::string(event_id);
    const ai::InteractionResult interaction = relationship_.trigger_event(event_id);
    result.success = interaction.success;
    result.dialogue_line = interaction.dialogue_line;
    result.rejection_reason = interaction.rejection_reason;
    if (!interaction.memory_created.id.empty()) {
        result.memory_id = interaction.memory_created.id;
    }
    if (interaction.success) {
        const scene::SceneStartResult scene = director_.start_scene(event_id);
        if (scene.success) {
            scene_active_ = true;
        }
    }
    return result;
}

bool ApartmentLifeSession::talk(std::string_view dialogue_node_id) {
    const ai::InteractionResult result = relationship_.talk(dialogue_node_id);
    return result.success;
}

bool ApartmentLifeSession::make_dialogue_choice(std::string_view node_id,
                                                  std::string_view choice_id) {
    const ai::InteractionResult result = relationship_.make_choice(node_id, choice_id);
    return result.success;
}

bool ApartmentLifeSession::change_outfit(std::string_view outfit_id) {
    if (!viewer_.change_outfit(outfit_id)) {
        return false;
    }
    ai::CharacterSimState* character = relationship_.character();
    if (character != nullptr) {
        character->current_outfit_id = std::string(outfit_id);
    }
    relationship_.trigger_outfit_dialogue();
    return true;
}

bool ApartmentLifeSession::change_hair(std::string_view hair_id) {
    return viewer_.change_hair(hair_id);
}

bool ApartmentLifeSession::change_expression(std::string_view expression_id) {
    return viewer_.change_expression(expression_id);
}

bool ApartmentLifeSession::randomize_outfit() {
    std::vector<std::string> outfits;
    for (int i = 1; i <= 20; ++i) {
        char id[32];
        std::snprintf(id, sizeof(id), "mira_outfit_%02d", i);
        if (content_.wardrobe().find(id) != nullptr) {
            outfits.push_back(id);
        }
    }
    if (outfits.empty()) {
        return false;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> dist(0, outfits.size() - 1);
    return change_outfit(outfits[dist(gen)]);
}

bool ApartmentLifeSession::enter_photo_mode(bool enabled) {
    return viewer_.enter_photo_mode(enabled);
}

bool ApartmentLifeSession::capture_photo(std::string_view path) {
    return viewer_.capture_photo(path);
}

bool ApartmentLifeSession::save_game() {
    save_data_ = save_system_.capture_from_relationship_sim(relationship_, config_.project_id);
    save_data_.save_slot = config_.save_slot;
    save_data_.character.character_id = config_.character_id;
    save_data_.character.outfit = viewer_.inspection().preview().outfit_id;
    save_data_.character.room = current_room_;
    save_data_.wardrobe.active_outfit_id = viewer_.inspection().preview().outfit_id;
    save_data_.apartment.apartment_id = config_.apartment_id;
    save_data_.apartment.active_room = current_room_;
    save_data_.timestamp = static_cast<u64>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
    return save_system_.save(save_path(), save_data_);
}

bool ApartmentLifeSession::load_game() {
    if (!save_system_.load(save_path(), save_data_)) {
        return false;
    }
    save_system_.apply_to_relationship_sim(relationship_, save_data_);
    if (!save_data_.apartment.active_room.empty()) {
        change_room(save_data_.apartment.active_room);
    }
    if (!save_data_.wardrobe.active_outfit_id.empty()) {
        viewer_.change_outfit(save_data_.wardrobe.active_outfit_id);
    }
    return true;
}

bool ApartmentLifeSession::auto_save() {
    return save_game();
}

bool ApartmentLifeSession::validate_content() const {
    polish::ValidationGate gate;
    const polish::ValidationGateResult result =
        gate.validate_for_export(config_.game_root, build::ExportProfile::WindowsDevelopment);
    return result.passed;
}

bool ApartmentLifeSession::export_windows_dev(std::string& output_path) {
    build::BuildPipeline pipeline;
    build::BuildPipelineConfig cfg;
    cfg.project_root = config_.game_root;
    cfg.project_name = config_.project_id;
    cfg.profile = build::ExportProfile::WindowsDevelopment;
    cfg.backup_root = config_.game_root + "/Backups";
    const build::BuildPipelineResult result = pipeline.export_project(cfg);
    if (!result.success) {
        return false;
    }
    output_path = result.output_path;
    return true;
}

bool ApartmentLifeSession::export_web_dev(std::string& output_path) {
    build::BuildPipeline pipeline;
    build::BuildPipelineConfig cfg;
    cfg.project_root = config_.game_root;
    cfg.project_name = config_.project_id;
    cfg.profile = build::ExportProfile::WebDevelopment;
    cfg.backup_root = config_.game_root + "/Backups";
    const build::BuildPipelineResult result = pipeline.export_project(cfg);
    if (!result.success) {
        return false;
    }
    output_path = result.output_path;
    return true;
}

std::vector<VerticalSliceStep> ApartmentLifeSession::run_vertical_slice() {
    std::vector<VerticalSliceStep> steps;
    auto record = [&](std::string_view phase, bool success, std::string detail = {}) {
        steps.push_back({std::string(phase), success, std::move(detail)});
    };

    record("init", initialized_);
    if (!initialized_) {
        return steps;
    }

    auto& input = viewer_.input();
    input.on_mouse_button(interaction::MouseButton::Right, true);
    input.on_mouse_move(80.0f, 30.0f);
    input.on_mouse_button(interaction::MouseButton::Right, false);
    input.on_mouse_scroll(-1.0f);
    update(0.016f);
    record("camera_orbit", true);

    record("select", viewer_.handle_select());
    record("interaction_ring", viewer_.open_interaction_ring());

    for (const char* room : kDemoRooms) {
        record(std::string("room_") + room, change_room(room));
    }

    record("outfit_change", change_outfit("mira_outfit_02"));
    record("hair_change", change_hair("mira_hair_loose"));
    record("expression_change", change_expression("soft_smile"));
    record("outfit_random", randomize_outfit());

    record("talk_greet", talk("greet_evening"));
    record("compliment", viewer_.execute_ring_action(interaction::RingActionType::Compliment));

    for (const char* event_id : kDemoEvents) {
        const EventPlayResult event = trigger_event(event_id);
        record(std::string("event_") + event_id, event.success, event.dialogue_line);
        if (scene_active_) {
            for (int i = 0; i < 5 && scene_active_; ++i) {
                update(0.5f);
            }
        }
    }

    record("living_room_interact", interact("sit_sofa"));
    record("wardrobe_interact", change_room("wardrobe") && interact("mirror_preview"));
    record("photo_mode", enter_photo_mode(true));

    record("save", save_game());
    record("load", load_game());

    std::string windows_out;
    std::string web_out;
    record("validate_content", validate_content());
    record("export_windows", export_windows_dev(windows_out), windows_out);
    record("export_web", export_web_dev(web_out), web_out);

    return steps;
}

} // namespace eve::gameplay
