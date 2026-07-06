#pragma once

#include <eve/ai/character_sim_state.hpp>

#include <optional>
#include <string>

namespace eve::ai {

enum class ScenePriority : u8 {
    Low = 0,
    Normal,
    High,
    Critical,
};

struct ActiveScene {
    std::string id;
    std::string description;
    ScenePriority priority = ScenePriority::Normal;
    bool interruptible = true;
};

struct InterruptionResult {
    bool interrupted = false;
    std::string resumed_scene_id;
    std::string message;
};

class SceneInterruptionHandler {
public:
    void begin_scene(ActiveScene scene);
    InterruptionResult request_interrupt(ActiveScene new_scene);
    void end_scene();
    [[nodiscard]] bool has_active_scene() const { return active_.has_value(); }
    [[nodiscard]] const ActiveScene* active_scene() const;

private:
    std::optional<ActiveScene> active_;
    std::optional<ActiveScene> suspended_;
};

} // namespace eve::ai
