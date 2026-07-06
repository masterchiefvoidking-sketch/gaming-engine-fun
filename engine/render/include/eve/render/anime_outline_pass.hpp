#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>

namespace eve::render {

struct OutlinePassSettings {
    bool enabled = true;
    f32 thickness = 1.2f;
    math::Vec3 color{0.12f, 0.08f, 0.14f};
    f32 depth_bias = 0.002f;
    bool silhouette_only = false;
};

class AnimeOutlinePass {
public:
    void set_settings(const OutlinePassSettings& settings);
    void set_thickness(f32 thickness);
    void begin_pass();
    void end_pass();

    [[nodiscard]] const OutlinePassSettings& settings() const { return settings_; }
    [[nodiscard]] bool is_active() const { return active_; }

private:
    OutlinePassSettings settings_{};
    bool active_ = false;
};

} // namespace eve::render
