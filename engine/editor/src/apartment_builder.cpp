#include <eve/core/logging/logger.hpp>
#include <eve/editor/apartment_builder.hpp>
#include <eve/world/interaction.hpp>

namespace eve::editor {

bool ApartmentBuilder::load_default_apartment(std::string_view data_root) {
    data_root_ = std::string(data_root);
    const std::string layout_path = data_root_ + "/apartments/default/layout.json";
    const std::string interactions_path = data_root_ + "/interactions";

    if (!loader_.load_layout(layout_path, layout_)) {
        EVE_LOG(Error, "Editor", "Failed to load apartment layout: ", layout_path);
        return false;
    }
    if (!interactions_.load_from_directory(interactions_path)) {
        EVE_LOG(Warn, "Editor", "No interaction definitions found in: ", interactions_path);
    }

    social_context_.simulation = &simulation_;
    social_context_.relationships = &relationships_;
    social_context_.schedules = &schedules_;
    social_context_.current_hour = simulation_.clock.hour;

    EVE_LOG(Info, "Editor", "Loaded apartment: ", layout_.display_name);
    return true;
}

bool ApartmentBuilder::build_scene() {
    registry_.register_component<world::InteractableComponent>();
    registry_.register_component<world::RoomComponent>();
    return loader_.spawn_interactables(layout_, world_, interactions_);
}

bool ApartmentBuilder::save_state(std::string_view path) const {
    const world::ApartmentSaveData data =
        persistence_.capture_from_world(layout_.id, const_cast<ecs::World&>(world_), simulation_);
    return persistence_.save(data, path);
}

bool ApartmentBuilder::load_state(std::string_view path) {
    world::ApartmentSaveData data;
    if (!persistence_.load(path, data)) {
        return false;
    }
    simulation_ = data.simulation;
    return persistence_.apply_to_world(data, world_);
}

} // namespace eve::editor
