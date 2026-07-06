#include <eve/scene/scene_director.hpp>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/core/logging/logger.hpp>

#include <nlohmann/json.hpp>

namespace eve::scene {

bool SceneDirector::initialize(std::string_view data_root,
                               ai::RelationshipSimulation& relationship_sim) {
    data_root_ = std::string(data_root);
    relationship_sim_ = &relationship_sim;
    const std::string base = data_root_ + "/scene_director";

    if (!scheduler_.load_templates(base + "/scene_templates.json")) {
        EVE_LOG(Warn, "SceneDirector", "Failed to load scene templates");
    }
    if (!conditions_.load_conditions(base + "/event_conditions.json")) {
        return false;
    }
    shots_.load_shots(base + "/camera_shots.json");
    animations_.load_cues(base + "/animation_cues.json");
    lighting_cues_.load_presets(base + "/lighting_presets.json");
    music_cues_.load_cues(base + "/music_cues.json");
    outcomes_.load_outcomes(base + "/emotional_outcomes.json");
    room_triggers_.load_triggers(base + "/room_triggers.json");

    if (scheduler_.find_template("rainy_movie_night") == nullptr) {
        scheduler_.load_templates(base + "/rainy_movie_night.json");
    }

    director_state_.scheduler.current_hour = 20.0f;
    director_state_.scheduler.runtime.weather = "rain";
    director_state_.scheduler.runtime.time_of_day = 20.0f;
    initialized_ = true;
    EVE_LOG(Info, "SceneDirector", "Scene director initialized");
    return true;
}

SceneStartResult SceneDirector::start_scene(std::string_view scene_id) {
    SceneStartResult result;
    if (!initialized_ || relationship_sim_ == nullptr) {
        result.rejection_reason = "Director not initialized";
        return result;
    }

    active_template_ = scheduler_.find_template(scene_id);
    if (active_template_ == nullptr) {
        result.rejection_reason = "Scene not found";
        return result;
    }

    if (scheduler_.is_on_cooldown(scene_id, director_state_.scheduler)) {
        result.rejection_reason = "Scene on cooldown";
        return result;
    }

    const SceneRuntimeContext ctx = build_runtime_context();
    if (!active_template_->condition_id.empty()) {
        const ConditionEvaluationResult cond =
            conditions_.evaluate(active_template_->condition_id, ctx);
        if (!cond.eligible) {
            result.rejection_reason = cond.rejection_reason;
            return result;
        }
    }

    if (!apply_privacy_gate(*active_template_)) {
        result.rejection_reason = "Privacy or consent gate failed";
        return result;
    }

    ai::CharacterSimState* character = relationship_sim_->character();
    if (character != nullptr) {
        character->current_room_id = active_template_->room_id;
    }

    if (!state_machine_.begin_scene(scene_id, active_template_->room_id)) {
        result.rejection_reason = "Could not begin scene state machine";
        return result;
    }
    state_machine_.transition_to(SceneState::Loading);

    if (lighting_ != nullptr && !active_template_->beats.empty()) {
        for (const SceneBeat& beat : active_template_->beats) {
            if (!beat.lighting_cue_id.empty()) {
                lighting_cues_.apply(beat.lighting_cue_id, *lighting_);
                break;
            }
        }
    }

    state_machine_.transition_to(SceneState::Entering);
    result.success = true;
    result.scene_id = std::string(scene_id);
    return result;
}

SceneStartResult SceneDirector::start_from_room(std::string_view room_id,
                                                std::string_view interaction_id) {
    const SceneRuntimeContext ctx = build_runtime_context();
    const RoomTriggerResult trigger =
        room_triggers_.find_eligible(room_id, interaction_id, ctx, conditions_);
    if (!trigger.triggered) {
        SceneStartResult result;
        result.rejection_reason = trigger.rejection_reason;
        return result;
    }
    return start_scene(trigger.scene_id);
}

SceneStepResult SceneDirector::advance(f32 delta_seconds) {
    SceneStepResult result;
    if (active_template_ == nullptr || !state_machine_.is_active()) {
        return result;
    }

    state_machine_.playback().elapsed_seconds += delta_seconds;
    shots_.update(delta_seconds);
    if (motion_ != nullptr) {
        motion_->update(delta_seconds);
    }
    if (expressions_ != nullptr) {
        expressions_->update(delta_seconds);
    }

    const u32 beat_index = state_machine_.playback().current_beat_index;
    if (beat_index >= active_template_->beats.size()) {
        if (!active_template_->default_outcome_id.empty() && relationship_sim_ != nullptr) {
            ai::CharacterSimState* character = relationship_sim_->character();
            ai::RelationshipModel* relationship = relationship_sim_->relationship();
            if (character != nullptr && relationship != nullptr) {
                const OutcomeApplicationResult outcome = outcomes_.apply(
                    active_template_->default_outcome_id, *character, *relationship,
                    relationship_sim_->memories(), 0);
                result.memory_id = outcome.memory_id;
            }
        }
        state_machine_.complete_scene();
        director_state_.completed_scenes.push_back(active_template_->id);
        director_state_.scheduler.runtime.completed_events.push_back(active_template_->id);
        if (active_template_->cooldown_hours > 0.0f) {
            scheduler_.set_cooldown(active_template_->id, active_template_->cooldown_hours,
                                    director_state_.scheduler);
        }
        result.completed = true;
        result.state = to_string(state_machine_.state());
        active_template_ = nullptr;
        return result;
    }

    const SceneBeat& beat = active_template_->beats[beat_index];
    if (state_machine_.playback().elapsed_seconds >= beat.start_time) {
        play_beat(beat);
        state_machine_.advance_beat(beat_index + 1);
        result.advanced = true;
        result.dialogue_line = state_machine_.playback().current_dialogue_line;
        result.camera_shot_id = state_machine_.playback().current_shot_id;
        result.state = to_string(state_machine_.state());
    }
    return result;
}

SceneStepResult SceneDirector::invite_to_scene(std::string_view scene_id) {
    const SceneStartResult started = start_scene(scene_id);
    SceneStepResult result;
    if (!started.success) {
        result.state = started.rejection_reason;
        return result;
    }
    return advance(0.0f);
}

bool SceneDirector::make_dialogue_choice(std::string_view node_id, std::string_view choice_id) {
    if (relationship_sim_ == nullptr) {
        return false;
    }
    const ai::InteractionResult choice = relationship_sim_->make_choice(node_id, choice_id);
    return choice.success;
}

bool SceneDirector::play_beat(const SceneBeat& beat) {
    if (relationship_sim_ == nullptr) {
        return false;
    }

    if (!beat.camera_shot_id.empty() && inspection_camera_ != nullptr) {
        shots_.play_shot(beat.camera_shot_id, romance_camera_);
        state_machine_.playback().current_shot_id = beat.camera_shot_id;
    }

    if (!beat.lighting_cue_id.empty() && lighting_ != nullptr) {
        lighting_cues_.apply(beat.lighting_cue_id, *lighting_);
    }

    if (!beat.music_cue_id.empty()) {
        const MusicCueResult music = music_cues_.play(beat.music_cue_id);
        (void)music;
    }

    if (motion_ != nullptr && expressions_ != nullptr) {
        if (!beat.animation_cue_id.empty()) {
            animations_.dispatch(beat.animation_cue_id, *motion_, *expressions_);
        } else if (!beat.expression_id.empty()) {
            expressions_->set_expression(beat.expression_id);
        }
    }

    const ai::DialogueContext ctx = relationship_sim_->build_dialogue_context();
    const DialogueBeatResult dialogue =
        dialogue_sequencer_.play_beat(beat, relationship_sim_->dialogue(), ctx);
    if (dialogue.success && !dialogue.line.empty()) {
        state_machine_.playback().current_dialogue_line = dialogue.line;
        if (ai::RelationshipModel* rel = relationship_sim_->relationship()) {
            rel->record_conversation(dialogue.line);
        }
    }

    if (!beat.outcome_id.empty()) {
        ai::CharacterSimState* character = relationship_sim_->character();
        ai::RelationshipModel* relationship = relationship_sim_->relationship();
        if (character != nullptr && relationship != nullptr) {
            outcomes_.apply(beat.outcome_id, *character, *relationship,
                            relationship_sim_->memories(), 0);
        }
    }

    if (beat.wait_for_choice) {
        state_machine_.transition_to(SceneState::WaitingChoice);
    } else {
        state_machine_.transition_to(SceneState::Playing);
    }
    return true;
}

bool SceneDirector::apply_privacy_gate(const SceneTemplate& tmpl) const {
    if (relationship_sim_ == nullptr) {
        return false;
    }
    const ai::CharacterSimState* character = relationship_sim_->character();
    if (character == nullptr) {
        return false;
    }
    const ai::ConsentCheckResult consent = boundaries_.check_action(
        tmpl.trigger_interaction.empty() ? "scene_start" : tmpl.trigger_interaction, *character,
        relationship_sim_->current_stage());
    if (!consent.allowed) {
        return false;
    }
    if (tmpl.room_id == "bedroom" || tmpl.room_id == "bathroom") {
        if (static_cast<u8>(character->privacy_level) < static_cast<u8>(ai::PrivacyLevel::SemiPrivate) &&
            relationship_sim_->current_stage() < ai::RelationshipStage::CloseFriend) {
            return false;
        }
    }
    return true;
}

SceneRuntimeContext SceneDirector::build_runtime_context() const {
    SceneRuntimeContext ctx = director_state_.scheduler.runtime;
    if (relationship_sim_ == nullptr) {
        return ctx;
    }
    const ai::CharacterSimState* character = relationship_sim_->character();
    if (character != nullptr) {
        ctx.active_room = character->current_room_id;
        ctx.active_outfit = character->current_outfit_id;
        ctx.stage = relationship_sim_->current_stage();
        ctx.mood = character->mood;
        ctx.trust = character->trust;
        ctx.affection = character->affection;
        ctx.comfort = character->comfort;
        ctx.energy = character->energy;
        ctx.stress = character->stress;
        ctx.consent_granted = character->consent == ai::ConsentState::Granted;
        ctx.privacy_ok =
            static_cast<u8>(character->privacy_level) >= static_cast<u8>(ai::PrivacyLevel::SemiPrivate);
    }
    const ai::RelationshipModel* rel = relationship_sim_->relationship();
    if (rel != nullptr) {
        ctx.known_memories = relationship_sim_->memories().all_memory_tags(*rel);
    }
    ctx.completed_events = director_state_.completed_scenes;
    ctx.story_flags = director_state_.unlocked_events;
    return ctx;
}

bool SceneDirector::save_state(std::string_view path) const {
    nlohmann::json json;
    json["current_hour"] = director_state_.scheduler.current_hour;
    json["weather"] = director_state_.scheduler.runtime.weather;
    json["completed_scenes"] = director_state_.completed_scenes;
    json["unlocked_dialogue"] = director_state_.unlocked_dialogue;
    json["unlocked_events"] = director_state_.unlocked_events;
    json["cooldowns"] = nlohmann::json::object();
    for (const auto& [scene_id, until] : director_state_.scheduler.cooldowns) {
        json["cooldowns"][scene_id] = until;
    }
    if (state_machine_.is_active()) {
        json["active_scene"] = state_machine_.playback().active_scene_id;
        json["beat_index"] = state_machine_.playback().current_beat_index;
        json["elapsed"] = state_machine_.playback().elapsed_seconds;
    }
    return FileSystem::write_text_file(path, json.dump(2));
}

bool SceneDirector::load_state(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        director_state_.scheduler.current_hour = json.value("current_hour", 20.0f);
        director_state_.scheduler.runtime.weather = json.value("weather", "rain");
        director_state_.scheduler.runtime.time_of_day = director_state_.scheduler.current_hour;
        director_state_.completed_scenes.clear();
        if (json.contains("completed_scenes")) {
            for (const auto& scene : json["completed_scenes"]) {
                director_state_.completed_scenes.push_back(scene.get<std::string>());
            }
        }
        director_state_.scheduler.runtime.completed_events = director_state_.completed_scenes;
        director_state_.scheduler.cooldowns.clear();
        if (json.contains("cooldowns")) {
            for (const auto& [key, value] : json["cooldowns"].items()) {
                director_state_.scheduler.cooldowns[key] = value.get<f32>();
            }
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

const SceneTemplate* SceneDirector::active_template() const {
    return active_template_;
}

void SceneDirector::attach_camera(render::InspectionCamera& camera) {
    inspection_camera_ = &camera;
    romance_camera_.attach(camera);
    shots_.load_shots(data_root_ + "/scene_director/camera_shots.json");
}

void SceneDirector::attach_lighting(world::AnimeRoomLightingSystem& lighting) {
    lighting_ = &lighting;
}

void SceneDirector::attach_motion(animation::AnimeMotionController& motion) {
    motion_ = &motion;
}

void SceneDirector::attach_expressions(human::AnimeExpressionSystem& expressions) {
    expressions_ = &expressions;
}

} // namespace eve::scene
