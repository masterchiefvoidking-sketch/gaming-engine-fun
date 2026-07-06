#include <eve/interaction/quality_settings.hpp>

#include <algorithm>

namespace eve::interaction {

void QualitySettings::apply_tier(QualityTier tier) {
    switch (tier) {
    case QualityTier::Low:
        resolution_scale = 0.75f;
        texture_quality = QualityTier::Low;
        shadow_quality = QualityTier::Low;
        hair_quality = QualityTier::Low;
        cloth_quality = QualityTier::Low;
        reflection_quality = QualityTier::Low;
        ambient_occlusion = false;
        bloom = false;
        depth_of_field = false;
        frame_cap = 60;
        break;
    case QualityTier::Medium:
        resolution_scale = 0.9f;
        texture_quality = QualityTier::Medium;
        shadow_quality = QualityTier::Medium;
        hair_quality = QualityTier::Medium;
        cloth_quality = QualityTier::Medium;
        reflection_quality = QualityTier::Low;
        ambient_occlusion = true;
        bloom = true;
        depth_of_field = false;
        frame_cap = 60;
        break;
    case QualityTier::High:
        resolution_scale = 1.0f;
        texture_quality = QualityTier::High;
        shadow_quality = QualityTier::High;
        hair_quality = QualityTier::High;
        cloth_quality = QualityTier::Medium;
        reflection_quality = QualityTier::Medium;
        ambient_occlusion = true;
        bloom = true;
        depth_of_field = true;
        frame_cap = 120;
        break;
    case QualityTier::Ultra:
        resolution_scale = 1.0f;
        texture_quality = QualityTier::Ultra;
        shadow_quality = QualityTier::Ultra;
        hair_quality = QualityTier::Ultra;
        cloth_quality = QualityTier::High;
        reflection_quality = QualityTier::High;
        ambient_occlusion = true;
        bloom = true;
        depth_of_field = true;
        frame_cap = 144;
        break;
    }
    battery_saver = false;
}

void QualitySettings::apply_battery_saver() {
    battery_saver = true;
    resolution_scale = std::min(resolution_scale, 0.8f);
    hair_quality = QualityTier::Low;
    cloth_quality = QualityTier::Low;
    reflection_quality = QualityTier::Low;
    ambient_occlusion = false;
    bloom = false;
    depth_of_field = false;
    frame_cap = 60;
}

} // namespace eve::interaction
