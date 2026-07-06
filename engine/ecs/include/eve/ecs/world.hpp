#pragma once

#include <algorithm>
#include <bitset>
#include <cstddef>
#include <eve/core/types.hpp>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace eve::ecs {

struct ComponentDescriptor {
    ComponentTypeId id = 0;
    std::type_index type_id{typeid(void)};
    std::size_t size = 0;
    std::size_t alignment = 0;
    void (*destroy)(void*) = nullptr;
    void (*copy_construct)(void*, const void*) = nullptr;
};

class ComponentRegistry {
public:
    template <typename T> ComponentTypeId register_component() {
        const std::type_index type_id = std::type_index(typeid(T));
        if (const auto it = type_to_id_.find(type_id); it != type_to_id_.end()) {
            return it->second;
        }
        const ComponentTypeId id = static_cast<ComponentTypeId>(descriptors_.size() + 1);
        descriptors_.push_back(ComponentDescriptor{
            id,
            type_id,
            sizeof(T),
            alignof(T),
            [](void* ptr) { static_cast<T*>(ptr)->~T(); },
            [](void* dst, const void* src) { new (dst) T(*static_cast<const T*>(src)); },
        });
        type_to_id_.emplace(type_id, id);
        return id;
    }

    [[nodiscard]] const ComponentDescriptor* descriptor(ComponentTypeId id) const {
        if (id == 0 || id > descriptors_.size()) {
            return nullptr;
        }
        return &descriptors_[id - 1];
    }

    [[nodiscard]] std::size_t count() const { return descriptors_.size(); }

private:
    std::vector<ComponentDescriptor> descriptors_;
    std::unordered_map<std::type_index, ComponentTypeId> type_to_id_;
};

struct EntityRecord {
    EntityId id = kInvalidEntity;
    bool alive = false;
    std::vector<ComponentTypeId> component_types;
    std::unordered_map<ComponentTypeId, std::unique_ptr<u8, std::function<void(u8*)>>> components;
};

class World {
public:
    explicit World(ComponentRegistry& registry);

    EntityId create_entity();
    void destroy_entity(EntityId entity);
    [[nodiscard]] bool is_alive(EntityId entity) const;

    template <typename T> T& add_component(EntityId entity);

    template <typename T> T* get_component(EntityId entity);

    template <typename T> const T* get_component(EntityId entity) const;

    template <typename T> bool has_component(EntityId entity) const;

    template <typename T> void remove_component(EntityId entity);

    template <typename Fn> void each(Fn&& fn);

    [[nodiscard]] std::size_t entity_count() const;

private:
    template <typename T> ComponentTypeId component_id() const;

    ComponentRegistry& registry_;
    std::unordered_map<EntityId, EntityRecord> entities_;
    EntityId next_id_ = 1;
};

template <typename T> T& World::add_component(EntityId entity) {
    EntityRecord& record = entities_.at(entity);
    const ComponentTypeId id = component_id<T>();
    if (record.components.contains(id)) {
        return *reinterpret_cast<T*>(record.components[id].get());
    }

    const auto* desc = registry_.descriptor(id);
    void (*destroy_fn)(void*) = desc->destroy;
    const std::size_t alignment = desc->alignment;
    std::function<void(u8*)> deleter = [destroy_fn, alignment](u8* ptr) {
        if (destroy_fn != nullptr) {
            destroy_fn(ptr);
        }
        ::operator delete(ptr, std::align_val_t{alignment});
    };

    void* raw = ::operator new(desc->size, std::align_val_t{alignment});
    std::unique_ptr<u8, std::function<void(u8*)>> storage(static_cast<u8*>(raw),
                                                          std::move(deleter));
    T* component = new (storage.get()) T();
    record.components.emplace(id, std::move(storage));
    record.component_types.push_back(id);
    return *component;
}

template <typename T> T* World::get_component(EntityId entity) {
    const auto it = entities_.find(entity);
    if (it == entities_.end() || !it->second.alive) {
        return nullptr;
    }
    const ComponentTypeId id = component_id<T>();
    const auto comp_it = it->second.components.find(id);
    if (comp_it == it->second.components.end()) {
        return nullptr;
    }
    return reinterpret_cast<T*>(comp_it->second.get());
}

template <typename T> const T* World::get_component(EntityId entity) const {
    return const_cast<World*>(this)->get_component<T>(entity);
}

template <typename T> bool World::has_component(EntityId entity) const {
    return get_component<T>(entity) != nullptr;
}

template <typename T> void World::remove_component(EntityId entity) {
    auto it = entities_.find(entity);
    if (it == entities_.end()) {
        return;
    }
    const ComponentTypeId id = component_id<T>();
    it->second.components.erase(id);
    auto& types = it->second.component_types;
    types.erase(std::remove(types.begin(), types.end(), id), types.end());
}

template <typename Fn> void World::each(Fn&& fn) {
    for (auto& [id, record] : entities_) {
        if (record.alive) {
            fn(id, record);
        }
    }
}

template <typename T> ComponentTypeId World::component_id() const {
    return const_cast<ComponentRegistry&>(registry_).register_component<T>();
}

} // namespace eve::ecs
