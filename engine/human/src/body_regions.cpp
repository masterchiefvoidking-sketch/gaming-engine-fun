#include <eve/core/filesystem/filesystem.hpp>
#include <eve/human/body_regions.hpp>

#include <nlohmann/json.hpp>

namespace eve::human {

const char* to_string(BodyRegion region) {
    switch (region) {
    case BodyRegion::Head:
        return "head";
    case BodyRegion::Hair:
        return "hair";
    case BodyRegion::Face:
        return "face";
    case BodyRegion::Neck:
        return "neck";
    case BodyRegion::Shoulders:
        return "shoulders";
    case BodyRegion::Arms:
        return "arms";
    case BodyRegion::Hands:
        return "hands";
    case BodyRegion::Torso:
        return "torso";
    case BodyRegion::Back:
        return "back";
    case BodyRegion::Waist:
        return "waist";
    case BodyRegion::Hips:
        return "hips";
    case BodyRegion::Legs:
        return "legs";
    case BodyRegion::Feet:
        return "feet";
    default:
        return "unknown";
    }
}

BodyRegion body_region_from_string(std::string_view value) {
    if (value == "head") return BodyRegion::Head;
    if (value == "hair") return BodyRegion::Hair;
    if (value == "face") return BodyRegion::Face;
    if (value == "neck") return BodyRegion::Neck;
    if (value == "shoulders") return BodyRegion::Shoulders;
    if (value == "arms") return BodyRegion::Arms;
    if (value == "hands") return BodyRegion::Hands;
    if (value == "torso") return BodyRegion::Torso;
    if (value == "back") return BodyRegion::Back;
    if (value == "waist") return BodyRegion::Waist;
    if (value == "hips") return BodyRegion::Hips;
    if (value == "legs") return BodyRegion::Legs;
    if (value == "feet") return BodyRegion::Feet;
    return BodyRegion::Torso;
}

bool load_body_volumes_from_file(std::string_view path, std::vector<BodyRegionVolume>& out_volumes) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        if (!json.contains("regions")) {
            return false;
        }
        out_volumes.clear();
        for (const auto& entry : json["regions"]) {
            BodyRegionVolume volume;
            volume.id = entry.value("id", "");
            volume.region = body_region_from_string(entry.value("region", "torso"));
            if (entry.contains("center")) {
                volume.center.x = entry["center"].value("x", 0.0f);
                volume.center.y = entry["center"].value("y", 0.0f);
                volume.center.z = entry["center"].value("z", 0.0f);
            }
            if (entry.contains("half_extents")) {
                volume.half_extents.x = entry["half_extents"].value("x", 0.1f);
                volume.half_extents.y = entry["half_extents"].value("y", 0.1f);
                volume.half_extents.z = entry["half_extents"].value("z", 0.1f);
            }
            out_volumes.push_back(std::move(volume));
        }
        return !out_volumes.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

} // namespace eve::human
