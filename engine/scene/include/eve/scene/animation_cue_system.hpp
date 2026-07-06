#pragma once

#include <eve/animation/anime_motion.hpp>
#include <eve/human/anime_expression_system.hpp>
#include <eve/scene/scene_types.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::scene {

struct CueDispatchResult {
    bool dispatched = false;
    std::string cue_id;
    std::string clip_id;
    std::string expression_id;
};

class AnimationCueSystem {
public:
    bool load_cues(std::string_view path);
    void register_cue(AnimationCueDefinition cue);

    CueDispatchResult dispatch(std::string_view cue_id, animation::AnimeMotionController& motion,
                               human::AnimeExpressionSystem& expressions) const;

    [[nodiscard]] const AnimationCueDefinition* find_cue(std::string_view cue_id) const;
    [[nodiscard]] std::size_t cue_count() const { return cues_.size(); }

private:
    std::unordered_map<std::string, AnimationCueDefinition> cues_;
};

} // namespace eve::scene
