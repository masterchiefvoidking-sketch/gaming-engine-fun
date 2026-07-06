#include <eve/human/anime_visual_pipeline.hpp>

#include <eve/core/logging/logger.hpp>

namespace eve::human {

AnimeVisualPipeline::AnimeVisualPipeline() : hair_(material_) {}

bool AnimeVisualPipeline::initialize(std::string_view data_root) {
    data_root_ = std::string(data_root);
    if (!character_.initialize()) {
        return false;
    }
    if (!material_.initialize()) {
        return false;
    }
    eyes_.initialize();
    hair_.initialize();
    romance_camera_.attach(character_.camera());

    const std::string anime_data = data_root_ + "/anime";
    material_.load_presets(anime_data + "/materials.json");
    material_.apply_preset("warm_romance");
    eyes_.load_presets(anime_data + "/eye_presets.json");
    eyes_.apply_preset("romance_sparkle");
    hair_.load_styles(anime_data + "/hair_styles.json");
    hair_.apply_style("long_straight");
    expressions_.load_presets(anime_data + "/expressions.json");
    expressions_.set_expression("soft_smile");
    character_style_.load_from_file(anime_data + "/character_preset.json");
    character_style_.apply_style("original_heroine");
    lighting_.load_presets(anime_data + "/room_lighting.json");
    lighting_.apply_preset("wardrobe_cozy_lamp");
    romance_camera_.load_presets(anime_data + "/romance_cameras.json");
    romance_camera_.apply_preset("full_body_wardrobe");

    wardrobe_.load_items(data_root_ + "/anime/wardrobe_items.json");

    mirror_.add_mirror({"wardrobe_mirror", {0.0f, 1.2f, -1.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 2.0f}});

    character_.camera().set_mode(render::CameraMode::Wardrobe);
    motion_.set_body_language(animation::AnimeBodyLanguageId::MirrorPose);
    initialized_ = true;
    EVE_LOG(Info, "AnimePipeline", "Anime visual pipeline initialized");
    return true;
}

void AnimeVisualPipeline::shutdown() {
    if (initialized_) {
        material_.shutdown();
        character_.shutdown();
        initialized_ = false;
    }
}

void AnimeVisualPipeline::update(f32 delta_seconds) {
    expressions_.update(delta_seconds);
    motion_.update(delta_seconds);
    eyes_.update(delta_seconds);
    hair_.update(delta_seconds, {0.0f, 1.65f, 0.0f});
    character_.update(delta_seconds);
    sync_expression_to_render();
}

void AnimeVisualPipeline::orbit_camera(f32 yaw, f32 pitch, f32 zoom) {
    render::OrbitCameraInput input{};
    input.orbit_delta_yaw = yaw;
    input.orbit_delta_pitch = pitch;
    input.zoom_delta = zoom;
    character_.camera().update(0.0f, input);
}

void AnimeVisualPipeline::pan_camera(f32 x, f32 y) {
    render::OrbitCameraInput input{};
    input.pan_delta = {x, y};
    character_.camera().update(0.0f, input);
}

bool AnimeVisualPipeline::change_outfit(std::string_view item_id) {
    if (!wardrobe_.can_equip(item_id, current_outfit_)) {
        return false;
    }
    current_outfit_ = wardrobe_.equip(item_id, current_outfit_);
    return true;
}

bool AnimeVisualPipeline::change_hair(std::string_view style_id) {
    return hair_.apply_style(style_id);
}

bool AnimeVisualPipeline::set_expression(std::string_view expression_id) {
    if (!expressions_.set_expression(expression_id)) {
        return false;
    }
    sync_expression_to_render();
    return true;
}

void AnimeVisualPipeline::set_blush_enabled(bool enabled) {
    blush_enabled_ = enabled;
    if (!enabled) {
        material_.set_blush(0.0f);
    } else {
        material_.set_blush(expressions_.active_channels().blush);
    }
}

void AnimeVisualPipeline::apply_camera_preset(std::string_view preset_id) {
    romance_camera_.apply_preset(preset_id);
}

bool AnimeVisualPipeline::capture_screenshot(std::string_view path) {
    character_.photo_mode().enable(true);
    character_.photo_mode().capture_screenshot(path);
    return true;
}

void AnimeVisualPipeline::sync_expression_to_render() {
    const AnimeExpressionChannels& ch = expressions_.active_channels();
    material_.set_blush(blush_enabled_ ? ch.blush : 0.0f);
    eyes_.set_emotion(ch.blush > 0.5f ? render::EyeEmotionShape::Shy : render::EyeEmotionShape::Neutral);
    if (ch.mouth_smile > 0.5f) {
        eyes_.set_emotion(render::EyeEmotionShape::Happy);
    }
    character_.facial().set_target("smile", ch.mouth_smile);
}

} // namespace eve::human
