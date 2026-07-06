#pragma once

#include <eve/animation/anime_motion.hpp>
#include <eve/human/anime_character_style.hpp>
#include <eve/human/anime_expression_system.hpp>
#include <eve/human/character_framework.hpp>
#include <eve/human/wardrobe_system.hpp>
#include <eve/render/anime_eye_system.hpp>
#include <eve/render/anime_hair_renderer.hpp>
#include <eve/render/anime_human_material.hpp>
#include <eve/render/anime_outline_pass.hpp>
#include <eve/render/mirror_renderer.hpp>
#include <eve/render/romance_camera.hpp>
#include <eve/world/anime_room_lighting.hpp>

#include <string>

namespace eve::human {

class AnimeVisualPipeline {
public:
    AnimeVisualPipeline();

    bool initialize(std::string_view data_root);
    void shutdown();
    void update(f32 delta_seconds);

    void orbit_camera(f32 yaw, f32 pitch, f32 zoom);
    void pan_camera(f32 x, f32 y);
    bool change_outfit(std::string_view item_id);
    bool change_hair(std::string_view style_id);
    bool set_expression(std::string_view expression_id);
    void set_blush_enabled(bool enabled);
    void apply_camera_preset(std::string_view preset_id);
    bool capture_screenshot(std::string_view path);

    [[nodiscard]] CharacterFramework& character() { return character_; }
    [[nodiscard]] AnimeExpressionSystem& expressions() { return expressions_; }
    [[nodiscard]] const AnimeExpressionSystem& expressions() const { return expressions_; }
    [[nodiscard]] animation::AnimeMotionController& motion() { return motion_; }
    [[nodiscard]] render::AnimeHumanMaterial& material() { return material_; }
    [[nodiscard]] const render::AnimeHumanMaterial& material() const { return material_; }
    [[nodiscard]] render::AnimeEyeSystem& eyes() { return eyes_; }
    [[nodiscard]] render::AnimeOutlinePass& outline() { return outline_; }
    [[nodiscard]] const world::AnimeRoomLightingPreset& lighting() const { return lighting_.active(); }

private:
    void sync_expression_to_render();

    CharacterFramework character_;
    WardrobeSystem wardrobe_;
    AnimeCharacterStyleSystem character_style_;
    AnimeExpressionSystem expressions_;
    animation::AnimeMotionController motion_;
    render::AnimeHumanMaterial material_;
    render::AnimeEyeSystem eyes_;
    render::AnimeHairRenderer hair_;
    render::AnimeOutlinePass outline_;
    render::RomanceCameraController romance_camera_;
    render::MirrorRenderer mirror_;
    world::AnimeRoomLightingSystem lighting_;
    LayeredOutfit current_outfit_;
    std::string data_root_;
    bool blush_enabled_ = true;
    bool initialized_ = false;
};

} // namespace eve::human
