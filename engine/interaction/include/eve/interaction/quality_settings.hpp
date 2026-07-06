#pragma once

#include <eve/core/types.hpp>

namespace eve::interaction {

enum class QualityTier : u8 {
    Low = 0,
    Medium,
    High,
    Ultra,
};

struct QualitySettings {
    f32 resolution_scale = 1.0f;
    QualityTier texture_quality = QualityTier::High;
    QualityTier shadow_quality = QualityTier::High;
    QualityTier hair_quality = QualityTier::High;
    QualityTier cloth_quality = QualityTier::Medium;
    QualityTier reflection_quality = QualityTier::Medium;
    bool ambient_occlusion = true;
    bool bloom = true;
    bool depth_of_field = true;
    u32 frame_cap = 144;
    bool battery_saver = false;

    void apply_tier(QualityTier tier);
    void apply_battery_saver();
};

} // namespace eve::interaction
