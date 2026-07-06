#pragma once

#include <eve/human/anime_visual_pipeline.hpp>
#include <eve/human/body_interaction_system.hpp>
#include <eve/human/body_regions.hpp>
#include <eve/interaction/camera_input_bridge.hpp>
#include <eve/interaction/input_controller.hpp>
#include <eve/interaction/photo_mode_controller.hpp>
#include <eve/render/inspection_camera.hpp>

#include <string>
#include <vector>

namespace eve::interaction {

struct InspectionPreviewState {
    std::string outfit_id;
    std::string hair_id;
    std::string expression_id;
    std::string pose_id;
    std::string lighting_preset = "studio_soft";
    human::BodyRegion focused_region = human::BodyRegion::Torso;
};

class CharacterInspectionMode {
public:
    bool initialize(std::string_view data_root);
    void shutdown();
    void update(f32 delta_seconds, InputController& input);

    bool focus_body_region(human::BodyRegion region);
    bool preview_outfit(std::string_view outfit_id);
    bool preview_hair(std::string_view hair_id);
    bool preview_expression(std::string_view expression_id);
    bool preview_pose(std::string_view pose_id);
    bool apply_lighting_preset(std::string_view preset_id);
    bool capture_screenshot(std::string_view path);

    [[nodiscard]] render::InspectionCamera& camera() { return camera_; }
    [[nodiscard]] human::AnimeVisualPipeline& pipeline() { return pipeline_; }
    [[nodiscard]] PhotoModeController& photo_mode() { return photo_mode_; }
    [[nodiscard]] const InspectionPreviewState& preview() const { return preview_; }
    [[nodiscard]] const std::vector<human::BodyRegionVolume>& body_volumes() const {
        return body_volumes_;
    }

    human::BodyRegionTarget raycast_body_region(const math::Vec3& origin,
                                                const math::Vec3& direction) const;

private:
    render::InspectionCamera camera_;
    human::AnimeVisualPipeline pipeline_;
    PhotoModeController photo_mode_;
    CameraInputBridge camera_bridge_;
    InspectionPreviewState preview_;
    std::vector<human::BodyRegionVolume> body_volumes_;
    human::BodyInteractionRegistry body_registry_;
    human::BodyInteractionSystem body_interactions_{body_registry_};
    bool initialized_ = false;
};

} // namespace eve::interaction
