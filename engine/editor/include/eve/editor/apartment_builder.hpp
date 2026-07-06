#pragma once

#include <eve/ai/relationship.hpp>
#include <eve/ai/schedule.hpp>
#include <eve/ai/social_ai.hpp>
#include <eve/world/apartment.hpp>
#include <eve/world/interaction_system.hpp>
#include <eve/world/persistence.hpp>
#include <eve/world/simulation.hpp>
#include <string>

namespace eve::editor {

class ApartmentBuilder {
public:
    bool load_default_apartment(std::string_view data_root);
    [[nodiscard]] const world::ApartmentLayout& layout() const { return layout_; }
    [[nodiscard]] world::InteractionRegistry& interactions() { return interactions_; }
    [[nodiscard]] world::ApartmentSimulation& simulation() { return simulation_; }
    [[nodiscard]] ai::RelationshipRegistry& relationships() { return relationships_; }
    [[nodiscard]] ai::ScheduleLibrary& schedules() { return schedules_; }
    [[nodiscard]] ecs::World& world() { return world_; }

    bool build_scene();
    bool save_state(std::string_view path) const;
    bool load_state(std::string_view path);

private:
    ecs::ComponentRegistry registry_;
    ecs::World world_{registry_};
    world::InteractionRegistry interactions_;
    world::InteractionSystem interaction_system_{interactions_};
    world::ApartmentLoader loader_;
    world::ApartmentLayout layout_;
    world::ApartmentSimulation simulation_;
    world::ApartmentPersistence persistence_;
    ai::RelationshipRegistry relationships_;
    ai::ScheduleLibrary schedules_;
    ai::SocialContext social_context_;
    ai::SocialAI social_ai_{&social_context_};
    std::string data_root_;
};

} // namespace eve::editor
