#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>
#include <string>
#include <vector>

namespace eve::render {

struct MirrorSurface {
    std::string id;
    math::Vec3 position{};
    math::Vec3 normal{0.0f, 0.0f, 1.0f};
    math::Vec2 size{1.0f, 2.0f};
    bool active = true;
};

class MirrorRenderer {
public:
    void add_mirror(MirrorSurface mirror);
    void set_active_mirror(std::string_view mirror_id);
    void render_reflection_stub();

    [[nodiscard]] bool is_active() const { return active_; }
    [[nodiscard]] const MirrorSurface* active_mirror() const;

private:
    std::vector<MirrorSurface> mirrors_;
    std::string active_mirror_id_;
    bool active_ = false;
};

} // namespace eve::render
