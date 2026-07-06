#include <eve/render/mirror_renderer.hpp>

namespace eve::render {

void MirrorRenderer::add_mirror(MirrorSurface mirror) {
    mirrors_.push_back(std::move(mirror));
}

void MirrorRenderer::set_active_mirror(std::string_view mirror_id) {
    active_mirror_id_ = std::string(mirror_id);
    active_ = true;
}

void MirrorRenderer::render_reflection_stub() {
    // Reflection pass hook — renders character into mirror RT when Vulkan path is active.
}

const MirrorSurface* MirrorRenderer::active_mirror() const {
    for (const MirrorSurface& mirror : mirrors_) {
        if (mirror.id == active_mirror_id_) {
            return &mirror;
        }
    }
    return nullptr;
}

} // namespace eve::render
