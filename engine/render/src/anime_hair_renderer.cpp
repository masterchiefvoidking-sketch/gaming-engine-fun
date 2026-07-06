#include <cmath>
#include <eve/core/filesystem/filesystem.hpp>
#include <eve/render/anime_hair_renderer.hpp>

#include <nlohmann/json.hpp>

namespace eve::render {

AnimeHairRenderer::AnimeHairRenderer(AnimeHumanMaterial& material) : material_(material) {}

const char* to_string(AnimeHairStyleId style) {
    switch (style) {
    case AnimeHairStyleId::LongStraight:
        return "long_straight";
    case AnimeHairStyleId::TwinTails:
        return "twin_tails";
    case AnimeHairStyleId::ShortBob:
        return "short_bob";
    case AnimeHairStyleId::Ponytail:
        return "ponytail";
    case AnimeHairStyleId::Wavy:
        return "wavy";
    case AnimeHairStyleId::Braids:
        return "braids";
    case AnimeHairStyleId::BangsSide:
        return "bangs_side";
    case AnimeHairStyleId::BangsBlunt:
        return "bangs_blunt";
    default:
        return "long_straight";
    }
}

AnimeHairStyleId hair_style_from_string(std::string_view value) {
    if (value == "twin_tails") return AnimeHairStyleId::TwinTails;
    if (value == "short_bob") return AnimeHairStyleId::ShortBob;
    if (value == "ponytail") return AnimeHairStyleId::Ponytail;
    if (value == "wavy") return AnimeHairStyleId::Wavy;
    if (value == "braids") return AnimeHairStyleId::Braids;
    if (value == "bangs_side") return AnimeHairStyleId::BangsSide;
    if (value == "bangs_blunt") return AnimeHairStyleId::BangsBlunt;
    return AnimeHairStyleId::LongStraight;
}

bool AnimeHairRenderer::initialize() {
    active_style_.id = "long_straight";
    active_style_.highlight_bands = {{0.25f, 0.55f, {1.0f, 0.92f, 0.95f}, 0.55f}};
    return true;
}

void AnimeHairRenderer::load_styles(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            AnimeHairStylePreset style;
            style.id = entry.value("id", "");
            style.style = hair_style_from_string(entry.value("style", "long_straight"));
            style.chunk_count = entry.value("chunk_count", 24u);
            style.physics_sway = entry.value("physics_sway", 0.35f);
            if (entry.contains("root_color")) {
                style.root_color.x = entry["root_color"].value("r", 0.18f);
                style.root_color.y = entry["root_color"].value("g", 0.12f);
                style.root_color.z = entry["root_color"].value("b", 0.22f);
            }
            if (entry.contains("tip_color")) {
                style.tip_color.x = entry["tip_color"].value("r", 0.45f);
                style.tip_color.y = entry["tip_color"].value("g", 0.28f);
                style.tip_color.z = entry["tip_color"].value("b", 0.38f);
            }
            register_style(std::move(style));
        }
    } catch (const nlohmann::json::exception&) {
    }
}

void AnimeHairRenderer::register_style(AnimeHairStylePreset style) {
    styles_[style.id] = std::move(style);
}

bool AnimeHairRenderer::apply_style(std::string_view style_id) {
    const auto it = styles_.find(std::string(style_id));
    if (it == styles_.end()) {
        return false;
    }
    active_style_ = it->second;
    return true;
}

void AnimeHairRenderer::set_mode(AnimeHairRenderMode mode) {
    state_.mode = mode;
}

void AnimeHairRenderer::update(f32 delta_seconds, const math::Vec3& head_position) {
    (void)head_position;
    simulation_time_ += delta_seconds;
    const f32 wind = std::sin(simulation_time_ * 2.0f) * state_.wind_response;
    state_.sway_amount = active_style_.physics_sway * wind;
    material_.set_mood_tint({1.0f, 1.0f - state_.sway_amount * 0.02f, 1.0f});
}

u32 AnimeHairRenderer::active_chunk_count() const {
    switch (state_.mode) {
    case AnimeHairRenderMode::Strand:
        return active_style_.chunk_count * 40;
    case AnimeHairRenderMode::Card:
        return active_style_.chunk_count * 4;
    default:
        return active_style_.chunk_count;
    }
}

} // namespace eve::render
