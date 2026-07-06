#include <eve/anime_demo/cozy_wardrobe_demo.hpp>

#include <eve/core/logging/logger.hpp>

namespace eve::anime_demo {

bool CozyWardrobeDemo::initialize(std::string_view data_root) {
    if (!pipeline_.initialize(data_root)) {
        return false;
    }
    pipeline_.change_outfit("cozy_sweater");
    pipeline_.change_outfit("plaid_skirt");
    pipeline_.change_outfit("mary_janes");
    pipeline_.change_hair("long_straight");
    pipeline_.set_expression("soft_smile");
    EVE_LOG(Info, "AnimeDemo", "Cozy Apartment Wardrobe Preview ready");
    return true;
}

void CozyWardrobeDemo::shutdown() {
    pipeline_.shutdown();
}

void CozyWardrobeDemo::update(f32 delta_seconds) {
    pipeline_.update(delta_seconds);
}

void CozyWardrobeDemo::orbit(f32 yaw, f32 pitch, f32 zoom) {
    pipeline_.orbit_camera(yaw, pitch, zoom);
}

void CozyWardrobeDemo::pan(f32 x, f32 y) {
    pipeline_.pan_camera(x, y);
}

bool CozyWardrobeDemo::switch_outfit(std::string_view item_id) {
    return pipeline_.change_outfit(item_id);
}

bool CozyWardrobeDemo::switch_hair(std::string_view style_id) {
    return pipeline_.change_hair(style_id);
}

bool CozyWardrobeDemo::switch_expression(std::string_view expression_id) {
    return pipeline_.set_expression(expression_id);
}

void CozyWardrobeDemo::toggle_blush() {
    blush_on_ = !blush_on_;
    pipeline_.set_blush_enabled(blush_on_);
}

void CozyWardrobeDemo::apply_face_closeup() {
    pipeline_.apply_camera_preset("face_closeup");
}

void CozyWardrobeDemo::apply_full_body() {
    pipeline_.apply_camera_preset("full_body_wardrobe");
}

bool CozyWardrobeDemo::take_screenshot(std::string_view path) {
    return pipeline_.capture_screenshot(path);
}

} // namespace eve::anime_demo
