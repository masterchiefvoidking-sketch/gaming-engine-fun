#include <eve/scene/animation_cue_system.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::scene {

namespace {

animation::AnimeBodyLanguageId parse_body_language(const std::string& value) {
    if (value == "shy") return animation::AnimeBodyLanguageId::Shy;
    if (value == "confident") return animation::AnimeBodyLanguageId::Confident;
    if (value == "happy") return animation::AnimeBodyLanguageId::Happy;
    if (value == "embarrassed") return animation::AnimeBodyLanguageId::Embarrassed;
    if (value == "mirror_pose") return animation::AnimeBodyLanguageId::MirrorPose;
    if (value == "outfit_walk") return animation::AnimeBodyLanguageId::OutfitPreviewWalk;
    return animation::AnimeBodyLanguageId::Neutral;
}

} // namespace

bool AnimationCueSystem::load_cues(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            AnimationCueDefinition cue;
            cue.id = entry.value("id", "");
            cue.label = entry.value("label", "");
            cue.clip_id = entry.value("clip", "");
            cue.expression_id = entry.value("expression", "");
            cue.duration_seconds = entry.value("duration", 1.0f);
            cue.blend_weight = entry.value("blend", 1.0f);
            register_cue(std::move(cue));
        }
        return !cues_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void AnimationCueSystem::register_cue(AnimationCueDefinition cue) {
    cues_[cue.id] = std::move(cue);
}

CueDispatchResult AnimationCueSystem::dispatch(std::string_view cue_id,
                                               animation::AnimeMotionController& motion,
                                               human::AnimeExpressionSystem& expressions) const {
    CueDispatchResult result;
    const AnimationCueDefinition* cue = find_cue(cue_id);
    if (cue == nullptr) {
        return result;
    }

    if (!cue->clip_id.empty()) {
        motion.trigger_reaction(cue->clip_id);
    } else if (cue->id == "sit" || cue->id == "lean") {
        motion.set_body_language(animation::AnimeBodyLanguageId::Shy);
    } else if (cue->id == "laugh" || cue->id == "smile") {
        motion.set_body_language(animation::AnimeBodyLanguageId::Happy);
    } else if (cue->id == "blush" || cue->id == "look_away") {
        motion.set_body_language(animation::AnimeBodyLanguageId::Embarrassed);
    } else if (cue->id == "mirror_pose") {
        motion.set_body_language(animation::AnimeBodyLanguageId::MirrorPose);
    } else if (cue->id == "outfit_preview_walk") {
        motion.set_body_language(animation::AnimeBodyLanguageId::OutfitPreviewWalk);
    } else {
        motion.set_body_language(parse_body_language(cue->id));
    }

    if (!cue->expression_id.empty()) {
        expressions.set_expression(cue->expression_id, cue->blend_weight);
    } else if (cue->id == "smile") {
        expressions.set_expression("soft_smile", cue->blend_weight);
    } else if (cue->id == "laugh") {
        expressions.set_expression("laugh", cue->blend_weight);
    } else if (cue->id == "blush") {
        expressions.set_expression("blushing", cue->blend_weight);
    } else if (cue->id == "eye_contact") {
        expressions.set_expression("romantic_gaze", cue->blend_weight);
    }

    result.dispatched = true;
    result.cue_id = cue->id;
    result.clip_id = cue->clip_id;
    result.expression_id = cue->expression_id;
    return result;
}

const AnimationCueDefinition* AnimationCueSystem::find_cue(std::string_view cue_id) const {
    const auto it = cues_.find(std::string(cue_id));
    if (it == cues_.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace eve::scene
