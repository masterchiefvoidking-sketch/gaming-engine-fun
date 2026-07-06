#pragma once

#include <eve/ai/relationship_simulation.hpp>
#include <eve/animation/anime_motion.hpp>
#include <eve/human/anime_expression_system.hpp>
#include <eve/render/inspection_camera.hpp>
#include <eve/scene/scene_director.hpp>
#include <eve/world/anime_room_lighting.hpp>

#include <string>
#include <vector>

namespace eve::movie_demo {

struct MovieNightStep {
    std::string phase;
    bool success = false;
    std::string dialogue;
    std::string camera_shot;
    std::string memory_id;
    std::string state;
};

class RainyMovieNightDemo {
public:
    bool initialize(std::string_view data_root);
    void shutdown();
    void update(f32 delta_seconds);

    MovieNightStep begin_scene();
    MovieNightStep invite_to_movie();
    MovieNightStep advance_scene();
    MovieNightStep recall_movie_memory();

    bool save_state(std::string_view path) const;
    bool load_state(std::string_view path);

    [[nodiscard]] scene::SceneDirector& director() { return director_; }
    [[nodiscard]] ai::RelationshipSimulation& relationship() { return relationship_; }
    [[nodiscard]] std::string summarize() const;
    [[nodiscard]] const std::vector<MovieNightStep>& history() const { return history_; }

private:
    MovieNightStep record(std::string_view phase, const scene::SceneStepResult& result);

    ai::RelationshipSimulation relationship_;
    scene::SceneDirector director_;
    render::InspectionCamera camera_;
    world::AnimeRoomLightingSystem lighting_;
    animation::AnimeMotionController motion_;
    human::AnimeExpressionSystem expressions_;
    std::vector<MovieNightStep> history_;
    std::string data_root_;
    bool scene_running_ = false;
    bool initialized_ = false;
};

} // namespace eve::movie_demo
