#include <algorithm>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/human/character_profile.hpp>
#include <nlohmann/json.hpp>

namespace eve::human {

namespace {

nlohmann::json vec3_to_json(const math::Vec3& v) {
    return {{"x", v.x}, {"y", v.y}, {"z", v.z}};
}

math::Vec3 vec3_from_json(const nlohmann::json& json, const math::Vec3& fallback) {
    if (!json.is_object()) {
        return fallback;
    }
    return {json.value("x", fallback.x), json.value("y", fallback.y), json.value("z", fallback.z)};
}

} // namespace

void CharacterCreator::set_profile(CharacterProfile profile) {
    profile_ = std::move(profile);
}

void CharacterCreator::apply_body_preset(BodyTypePreset preset) {
    profile_.body.body_preset = preset;
    switch (preset) {
    case BodyTypePreset::Slim:
        profile_.body.shoulder_width = 0.4f;
        profile_.body.bust = 0.45f;
        profile_.body.waist = 0.35f;
        profile_.body.hip = 0.4f;
        profile_.body.muscle_definition = 0.2f;
        break;
    case BodyTypePreset::Athletic:
        profile_.body.shoulder_width = 0.55f;
        profile_.body.muscle_definition = 0.7f;
        profile_.body.waist = 0.4f;
        break;
    case BodyTypePreset::Curvy:
        profile_.body.bust = 0.65f;
        profile_.body.hip = 0.7f;
        profile_.body.waist = 0.45f;
        break;
    case BodyTypePreset::Petite:
        profile_.body.height = 1.55f;
        profile_.body.shoulder_width = 0.38f;
        break;
    case BodyTypePreset::Tall:
        profile_.body.height = 1.78f;
        profile_.body.leg_length = 0.65f;
        break;
    default:
        break;
    }
}

void CharacterCreator::set_morph(std::string_view key, f32 value) {
    profile_.expression_presets[std::string(key)] = std::clamp(value, 0.0f, 1.0f);
}

f32 CharacterCreator::morph(std::string_view key) const {
    const auto it = profile_.expression_presets.find(std::string(key));
    if (it == profile_.expression_presets.end()) {
        return 0.0f;
    }
    return it->second;
}

math::Mat4 CharacterCreator::compute_body_transform() const {
    const f32 height_scale = profile_.body.height / 1.68f;
    const f32 width_scale = 0.9f + profile_.body.weight * 0.2f;
    return math::Mat4::scale({width_scale, height_scale, width_scale});
}

bool CharacterCreator::save_to_file(std::string_view path) const {
    nlohmann::json json;
    json["id"] = profile_.id;
    json["display_name"] = profile_.display_name;
    json["body"] = {
        {"height", profile_.body.height},
        {"weight", profile_.body.weight},
        {"shoulder_width", profile_.body.shoulder_width},
        {"bust", profile_.body.bust},
        {"waist", profile_.body.waist},
        {"hip", profile_.body.hip},
        {"muscle_definition", profile_.body.muscle_definition},
        {"age_appearance", profile_.body.age_appearance},
    };
    json["skin"] = {
        {"tone", vec3_to_json(profile_.skin.skin_tone)},
        {"freckles", profile_.skin.freckles},
        {"beauty_marks", profile_.skin.beauty_marks},
        {"subsurface_strength", profile_.skin.subsurface_strength},
    };
    json["face"] = {
        {"face_shape", profile_.face.face_shape},
        {"jaw", profile_.face.jaw},
        {"lips", profile_.face.lips},
        {"eyes", profile_.face.eye_size},
        {"makeup", profile_.face.makeup_intensity},
    };
    json["hair"] = {
        {"style_id", profile_.hair.style_id},
        {"length", profile_.hair.length},
        {"root_color", vec3_to_json(profile_.hair.root_color)},
    };
    return FileSystem::write_text_file(path, json.dump(2));
}

bool CharacterCreator::load_from_file(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        profile_.id = json.value("id", "character");
        profile_.display_name = json.value("display_name", profile_.id);
        if (json.contains("body")) {
            const auto& body = json["body"];
            profile_.body.height = body.value("height", profile_.body.height);
            profile_.body.weight = body.value("weight", profile_.body.weight);
            profile_.body.bust = body.value("bust", profile_.body.bust);
            profile_.body.waist = body.value("waist", profile_.body.waist);
            profile_.body.hip = body.value("hip", profile_.body.hip);
            profile_.body.muscle_definition =
                body.value("muscle_definition", profile_.body.muscle_definition);
        }
        if (json.contains("skin")) {
            profile_.skin.skin_tone = vec3_from_json(json["skin"]["tone"], profile_.skin.skin_tone);
            profile_.skin.freckles = json["skin"].value("freckles", profile_.skin.freckles);
        }
        if (json.contains("hair")) {
            profile_.hair.style_id = json["hair"].value("style_id", profile_.hair.style_id);
            profile_.hair.length = json["hair"].value("length", profile_.hair.length);
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

} // namespace eve::human
