#pragma once

#include <eve/content/content_database.hpp>
#include <eve/content/content_types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::content {

struct FurniturePlacement {
    std::string furniture_id;
    std::string room_id;
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 rotation_y = 0.0f;
};

struct ApartmentContentDefinition {
    ContentMetadata metadata;
    std::vector<std::string> room_ids;
    std::vector<FurniturePlacement> furniture;
    std::vector<std::string> lighting_preset_ids;
};

struct ObjectInteractionDef {
    std::string id;
    std::string label;
    std::string animation_id;
    std::string sound_id;
};

struct ObjectContentDefinition {
    ContentMetadata metadata;
    std::vector<ObjectInteractionDef> interactions;
    std::string physics_profile;
    bool blocks_navigation = false;
    bool contributes_lighting = false;
    std::string state_machine_id;
};

class ApartmentContentLibrary {
public:
    bool load_apartment(std::string_view path);
    bool load_furniture(std::string_view path);
    void register_apartment(ApartmentContentDefinition def);
    void register_furniture(ObjectContentDefinition def);
    [[nodiscard]] ApartmentContentDefinition* find_apartment(std::string_view id);
    [[nodiscard]] ObjectContentDefinition* find_furniture(std::string_view id);
    [[nodiscard]] std::size_t apartment_count() const { return apartments_.size(); }
    [[nodiscard]] std::size_t furniture_count() const { return furniture_.size(); }

private:
    std::unordered_map<std::string, ApartmentContentDefinition> apartments_;
    std::unordered_map<std::string, ObjectContentDefinition> furniture_;
};

} // namespace eve::content
