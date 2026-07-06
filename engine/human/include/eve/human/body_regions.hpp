#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>

#include <string>
#include <vector>

namespace eve::human {

enum class BodyRegion : u8 {
    Head = 0,
    Hair,
    Face,
    Neck,
    Shoulders,
    Arms,
    Hands,
    Torso,
    Back,
    Waist,
    Hips,
    Legs,
    Feet,
    Count
};

enum class ContentRating : u8 {
    General = 0,
    Mature,
    AdultsOnly,
};

enum class ConsentState : u8 {
    Unknown = 0,
    Granted,
    Denied,
    Revoked,
};

struct BodyRegionVolume {
    BodyRegion region = BodyRegion::Head;
    std::string id;
    math::Vec3 center{};
    math::Vec3 half_extents{0.1f, 0.1f, 0.1f};
    math::Vec3 focus_offset{};
    bool hoverable = true;
    bool selectable = true;
};

struct BodyRegionTarget {
    BodyRegion region = BodyRegion::Head;
    std::string region_id;
    bool hovered = false;
    bool selected = false;
    f32 distance = 0.0f;
};

const char* to_string(BodyRegion region);
BodyRegion body_region_from_string(std::string_view value);
bool load_body_volumes_from_file(std::string_view path, std::vector<BodyRegionVolume>& out_volumes);

} // namespace eve::human
