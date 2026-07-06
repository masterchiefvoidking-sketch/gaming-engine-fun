#include <algorithm>
#include <eve/ecs/world.hpp>

namespace eve::ecs {

World::World(ComponentRegistry& registry)
    : registry_(registry) {}

EntityId World::create_entity() {
    const EntityId id = next_id_++;
    EntityRecord record{};
    record.id = id;
    record.alive = true;
    entities_.emplace(id, std::move(record));
    return id;
}

void World::destroy_entity(EntityId entity) {
    auto it = entities_.find(entity);
    if (it == entities_.end()) {
        return;
    }
    it->second.components.clear();
    it->second.component_types.clear();
    it->second.alive = false;
    entities_.erase(it);
}

bool World::is_alive(EntityId entity) const {
    const auto it = entities_.find(entity);
    return it != entities_.end() && it->second.alive;
}

std::size_t World::entity_count() const {
    return entities_.size();
}

} // namespace eve::ecs
