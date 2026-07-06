#pragma once

#include <eve/ai/relationship_simulation.hpp>

#include <string>
#include <vector>

namespace eve::rainy_demo {

struct DemoStepResult {
    std::string action;
    bool success = false;
    std::string dialogue;
    std::string rejection;
    std::string stage;
    std::string mood;
};

class RainyEveningDemo {
public:
    bool initialize(std::string_view data_root);
    void shutdown();

    DemoStepResult talk_in_living_room();
    DemoStepResult offer_compliment();
    DemoStepResult invite_to_dinner();
    DemoStepResult trigger_outfit_dialogue();
    DemoStepResult trigger_rainy_event();
    DemoStepResult try_blocked_action(std::string_view action_id);
    DemoStepResult recall_memory(std::string_view tag);

    bool save_state(std::string_view path) const;
    bool load_state(std::string_view path);

    [[nodiscard]] ai::RelationshipSimulation& simulation() { return simulation_; }
    [[nodiscard]] const ai::RelationshipSimulation& simulation() const { return simulation_; }
    [[nodiscard]] std::string summarize() const;
    [[nodiscard]] const std::vector<DemoStepResult>& history() const { return history_; }

private:
    DemoStepResult record(std::string_view action, const ai::InteractionResult& result);

    ai::RelationshipSimulation simulation_;
    std::string data_root_;
    std::vector<DemoStepResult> history_;
    bool initialized_ = false;
};

} // namespace eve::rainy_demo
