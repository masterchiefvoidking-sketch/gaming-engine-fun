#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>
#include <eve/render/anime_human_material.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::render {

enum class AnimeHairStyleId : u8 {
    LongStraight = 0,
    TwinTails,
    ShortBob,
    Ponytail,
    Wavy,
    Braids,
    BangsSide,
    BangsBlunt,
    Count
};

enum class AnimeHairRenderMode : u8 {
    Chunk = 0,
    Card,
    Strand,
};

struct AnimeHairHighlightBand {
    f32 start = 0.3f;
    f32 end = 0.7f;
    math::Vec3 color{1.0f, 0.95f, 0.88f};
    f32 intensity = 0.6f;
};

struct AnimeHairStylePreset {
    std::string id;
    AnimeHairStyleId style = AnimeHairStyleId::LongStraight;
    math::Vec3 root_color{0.18f, 0.12f, 0.22f};
    math::Vec3 tip_color{0.45f, 0.28f, 0.38f};
    std::vector<AnimeHairHighlightBand> highlight_bands;
    f32 physics_sway = 0.35f;
    u32 chunk_count = 24;
};

struct AnimeHairState {
    AnimeHairRenderMode mode = AnimeHairRenderMode::Chunk;
    f32 wind_response = 0.25f;
    f32 sway_amount = 0.0f;
    math::Vec3 wind_direction{0.3f, 0.0f, 0.1f};
    bool collision_enabled = true;
};

class AnimeHairRenderer {
public:
    explicit AnimeHairRenderer(AnimeHumanMaterial& material);

    bool initialize();
    void load_styles(std::string_view path);
    void register_style(AnimeHairStylePreset style);
    bool apply_style(std::string_view style_id);
    void set_mode(AnimeHairRenderMode mode);
    void update(f32 delta_seconds, const math::Vec3& head_position);

    [[nodiscard]] const AnimeHairStylePreset& active_style() const { return active_style_; }
    [[nodiscard]] const AnimeHairState& state() const { return state_; }
    [[nodiscard]] u32 active_chunk_count() const;

private:
    AnimeHumanMaterial& material_;
    AnimeHairState state_{};
    AnimeHairStylePreset active_style_{};
    std::unordered_map<std::string, AnimeHairStylePreset> styles_;
    f32 simulation_time_ = 0.0f;
};

AnimeHairStyleId hair_style_from_string(std::string_view value);
const char* to_string(AnimeHairStyleId style);

} // namespace eve::render
