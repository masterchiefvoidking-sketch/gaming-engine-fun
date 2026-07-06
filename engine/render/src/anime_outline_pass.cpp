#include <algorithm>
#include <eve/render/anime_outline_pass.hpp>

namespace eve::render {

void AnimeOutlinePass::set_settings(const OutlinePassSettings& settings) {
    settings_ = settings;
}

void AnimeOutlinePass::set_thickness(f32 thickness) {
    settings_.thickness = std::clamp(thickness, 0.5f, 4.0f);
}

void AnimeOutlinePass::begin_pass() {
    active_ = settings_.enabled;
}

void AnimeOutlinePass::end_pass() {
    active_ = false;
}

} // namespace eve::render
