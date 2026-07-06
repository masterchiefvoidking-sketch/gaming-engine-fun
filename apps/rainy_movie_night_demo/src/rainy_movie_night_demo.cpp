#include <eve/movie_demo/rainy_movie_night_demo.hpp>
#include <eve/core/logging/logger.hpp>

namespace eve::movie_demo {

bool RainyMovieNightDemo::initialize(std::string_view data_root) {
    data_root_ = std::string(data_root);
    if (!relationship_.initialize(data_root_)) {
        return false;
    }
    if (!director_.initialize(data_root_, relationship_)) {
        return false;
    }

    director_.attach_camera(camera_);
    director_.attach_lighting(lighting_);
    director_.attach_motion(motion_);
    director_.attach_expressions(expressions_);

    lighting_.load_presets(data_root_ + "/apartment/anime_lighting.json");
    expressions_.load_presets(data_root_ + "/anime/expressions.json");

    ai::CharacterSimState* character = relationship_.character();
    if (character != nullptr) {
        character->current_room_id = "living_room";
        character->current_outfit_id = "cozy_sweater";
        character->consent = ai::ConsentState::Granted;
        character->trust = 0.2f;
        character->familiarity = 0.15f;
        character->affection = 0.12f;
        character->comfort = 0.4f;
    }

    director_.director_state().scheduler.runtime.weather = "rain";
    director_.director_state().scheduler.runtime.time_of_day = 20.0f;
    director_.director_state().scheduler.current_hour = 20.0f;

    initialized_ = true;
    EVE_LOG(Info, "MovieDemo", "Rainy Movie Night demo initialized");
    return true;
}

void RainyMovieNightDemo::shutdown() {
    history_.clear();
    scene_running_ = false;
    initialized_ = false;
}

void RainyMovieNightDemo::update(f32 delta_seconds) {
    if (scene_running_) {
        advance_scene();
    }
    motion_.update(delta_seconds);
    expressions_.update(delta_seconds);
}

MovieNightStep RainyMovieNightDemo::record(std::string_view phase,
                                           const scene::SceneStepResult& result) {
    MovieNightStep step;
    step.phase = std::string(phase);
    step.success = result.advanced || result.completed;
    step.dialogue = result.dialogue_line;
    step.camera_shot = result.camera_shot_id;
    step.memory_id = result.memory_id;
    step.state = result.state;
    history_.push_back(step);
    return step;
}

MovieNightStep RainyMovieNightDemo::begin_scene() {
    const scene::SceneStartResult started =
        director_.start_from_room("living_room", "invite_movie");
    MovieNightStep step;
    step.phase = "begin";
    step.success = started.success;
    step.state = started.scene_id;
    if (!started.success) {
        step.dialogue = started.rejection_reason;
        history_.push_back(step);
        return step;
    }
    scene_running_ = true;
    return advance_scene();
}

MovieNightStep RainyMovieNightDemo::invite_to_movie() {
    return begin_scene();
}

MovieNightStep RainyMovieNightDemo::advance_scene() {
    const scene::SceneStepResult result = director_.advance(1.0f);
    if (result.completed) {
        scene_running_ = false;
    }
    return record("beat", result);
}

MovieNightStep RainyMovieNightDemo::recall_movie_memory() {
    MovieNightStep step;
    step.phase = "recall";
    const ai::RelationshipModel* rel = relationship_.relationship();
    if (rel == nullptr) {
        history_.push_back(step);
        return step;
    }
    ai::RelationshipModel mutable_rel = *rel;
    const ai::MemoryRecallResult recall =
        relationship_.memories().recall_by_tag(mutable_rel, "first_movie_night");
    step.success = recall.found;
    step.dialogue = recall.callback_line;
    step.memory_id = recall.found ? "first_movie_night" : "";
    history_.push_back(step);
    return step;
}

bool RainyMovieNightDemo::save_state(std::string_view path) const {
    return director_.save_state(path) && relationship_.save_state(std::string(path) + ".rel");
}

bool RainyMovieNightDemo::load_state(std::string_view path) {
    return director_.load_state(path) && relationship_.load_state(std::string(path) + ".rel");
}

std::string RainyMovieNightDemo::summarize() const {
    const ai::CharacterSimState* character = relationship_.character();
    const ai::RelationshipModel* rel = relationship_.relationship();
    if (character == nullptr || rel == nullptr) {
        return "Uninitialized";
    }
    return std::string("Rainy Movie Night — ") + ai::to_string(relationship_.current_stage()) +
           ", trust " + std::to_string(character->trust) + ", affection " +
           std::to_string(character->affection) + ", memories " +
           std::to_string(rel->shared_memories.size());
}

} // namespace eve::movie_demo
