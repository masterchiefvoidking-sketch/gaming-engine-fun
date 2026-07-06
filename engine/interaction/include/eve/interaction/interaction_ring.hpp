#pragma once

#include <eve/human/interaction_rules.hpp>
#include <eve/interaction/input_types.hpp>

#include <string>
#include <vector>

namespace eve::interaction {

enum class RingActionType : u8 {
    Look = 0,
    Talk,
    Compliment,
    ChangeOutfit,
    Pose,
    UseObject,
    Inspect,
    Cancel,
};

struct InteractionRingAction {
    RingActionType type = RingActionType::Cancel;
    std::string id;
    std::string label;
    bool enabled = true;
    std::string disabled_reason;
};

struct InteractionRingContext {
    human::InteractionContextSnapshot interaction;
    std::string target_id;
    std::string target_label;
    bool is_character = false;
    bool is_object = false;
    human::BodyRegion body_region = human::BodyRegion::Torso;
    std::string current_activity = "idle";
};

class InteractionRing {
public:
    [[nodiscard]] std::vector<InteractionRingAction>
    build_actions(const InteractionRingContext& context) const;

    [[nodiscard]] bool is_action_available(RingActionType type,
                                           const InteractionRingContext& context) const;

    [[nodiscard]] InteractionRingAction
    action_for_type(RingActionType type, const InteractionRingContext& context) const;
};

} // namespace eve::interaction
