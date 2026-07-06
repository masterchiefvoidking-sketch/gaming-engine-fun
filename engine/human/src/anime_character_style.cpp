#include <eve/core/filesystem/filesystem.hpp>
#include <eve/human/anime_character_style.hpp>

#include <nlohmann/json.hpp>

namespace eve::human {

bool AnimeCharacterStyleSystem::load_from_file(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        AnimeCharacterStyle style;
        style.id = json.value("id", "default_anime");
        style.display_name = json.value("display_name", style.id);
        style.default_hair_style = json.value("default_hair_style", "long_straight");
        style.default_material = json.value("default_material", "warm_romance");
        if (json.contains("body")) {
            const auto& body = json["body"];
            style.body.height = body.value("height", 1.58f);
            style.body.head_to_body_ratio = body.value("head_to_body_ratio", 0.22f);
            style.body.shoulder_width = body.value("shoulder_width", 0.38f);
            style.body.waist = body.value("waist", 0.34f);
            style.body.hip = body.value("hip", 0.42f);
            style.body.bust = body.value("bust", 0.40f);
            style.body.leg_length = body.value("leg_length", 0.52f);
        }
        register_style(std::move(style));
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void AnimeCharacterStyleSystem::register_style(AnimeCharacterStyle style) {
    styles_[style.id] = std::move(style);
}

bool AnimeCharacterStyleSystem::apply_style(std::string_view style_id) {
    const auto it = styles_.find(std::string(style_id));
    if (it == styles_.end()) {
        return false;
    }
    active_ = it->second;
    return true;
}

void AnimeCharacterStyleSystem::apply_silhouette_preset(AnimeSilhouettePreset preset) {
    active_.body.silhouette = preset;
    switch (preset) {
    case AnimeSilhouettePreset::Petite:
        active_.body.height = 1.52f;
        active_.body.head_to_body_ratio = 0.24f;
        active_.body.shoulder_width = 0.34f;
        break;
    case AnimeSilhouettePreset::Curvy:
        active_.body.hip = 0.48f;
        active_.body.bust = 0.46f;
        active_.body.waist = 0.32f;
        break;
    case AnimeSilhouettePreset::TallElegant:
        active_.body.height = 1.68f;
        active_.body.leg_length = 0.58f;
        break;
    default:
        break;
    }
}

math::Mat4 AnimeCharacterStyleSystem::compute_transform() const {
    const f32 scale = active_.body.height / 1.58f;
    const f32 head_scale = active_.body.head_to_body_ratio / 0.22f;
    return math::Mat4::scale({scale, scale * head_scale, scale});
}

} // namespace eve::human
