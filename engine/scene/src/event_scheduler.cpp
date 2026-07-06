#include <eve/scene/event_scheduler.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::scene {

namespace {

SceneEventType parse_event_type(const std::string& value) {
    if (value == "morning_greeting") return SceneEventType::MorningGreeting;
    if (value == "coffee_together") return SceneEventType::CoffeeTogether;
    if (value == "getting_ready") return SceneEventType::GettingReady;
    if (value == "wardrobe_preview") return SceneEventType::WardrobePreview;
    if (value == "cooking_together") return SceneEventType::CookingTogether;
    if (value == "shared_dinner") return SceneEventType::SharedDinner;
    if (value == "movie_night") return SceneEventType::MovieNight;
    if (value == "rainy_evening_talk") return SceneEventType::RainyEveningTalk;
    if (value == "balcony_conversation") return SceneEventType::BalconyConversation;
    if (value == "bedroom_goodnight") return SceneEventType::BedroomGoodnight;
    if (value == "living_room_relaxation") return SceneEventType::LivingRoomRelaxation;
    if (value == "mirror_outfit_reaction") return SceneEventType::MirrorOutfitReaction;
    if (value == "date_preparation") return SceneEventType::DatePreparation;
    if (value == "apology_conversation") return SceneEventType::ApologyConversation;
    if (value == "comfort_conversation") return SceneEventType::ComfortConversation;
    if (value == "celebration") return SceneEventType::Celebration;
    if (value == "gift_reaction") return SceneEventType::GiftReaction;
    if (value == "photo_mode_pose") return SceneEventType::PhotoModePose;
    return SceneEventType::Custom;
}

} // namespace

bool EventScheduler::load_templates(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            SceneTemplate tmpl;
            tmpl.id = entry.value("id", "");
            tmpl.label = entry.value("label", "");
            tmpl.type = parse_event_type(entry.value("type", "custom"));
            tmpl.room_id = entry.value("room", "");
            tmpl.trigger_interaction = entry.value("interaction", "");
            tmpl.condition_id = entry.value("condition", "");
            tmpl.default_outcome_id = entry.value("outcome", "");
            tmpl.memory_template_id = entry.value("memory", "");
            tmpl.memory_template_id = entry.value("memory", "");
            tmpl.cooldown_hours = entry.value("cooldown_hours", 0.0f);
            tmpl.one_time = entry.value("one_time", false);
            if (entry.contains("beats")) {
                for (const auto& beat_json : entry["beats"]) {
                    SceneBeat beat;
                    beat.start_time = beat_json.value("time", 0.0f);
                    beat.dialogue_node_id = beat_json.value("dialogue", "");
                    beat.camera_shot_id = beat_json.value("shot", "");
                    beat.animation_cue_id = beat_json.value("animation", "");
                    beat.expression_id = beat_json.value("expression", "");
                    beat.lighting_cue_id = beat_json.value("lighting", "");
                    beat.music_cue_id = beat_json.value("music", "");
                    beat.outcome_id = beat_json.value("outcome", "");
                    beat.wait_for_choice = beat_json.value("wait_choice", false);
                    tmpl.beats.push_back(std::move(beat));
                }
            }
            register_template(std::move(tmpl));
        }
        return !templates_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void EventScheduler::register_template(SceneTemplate tmpl) {
    templates_[tmpl.id] = std::move(tmpl);
}

std::vector<ScheduledEventCandidate> EventScheduler::eligible_events(
    const SchedulerContext& ctx, const SceneConditionEvaluator& conditions) const {
    std::vector<ScheduledEventCandidate> candidates;
    for (const auto& [id, tmpl] : templates_) {
        (void)id;
        if (is_on_cooldown(tmpl.id, ctx)) {
            continue;
        }
        if (tmpl.one_time) {
            if (std::find(ctx.runtime.completed_events.begin(), ctx.runtime.completed_events.end(),
                          tmpl.id) != ctx.runtime.completed_events.end()) {
                continue;
            }
        }
        if (!tmpl.condition_id.empty()) {
            const ConditionEvaluationResult cond =
                conditions.evaluate(tmpl.condition_id, ctx.runtime);
            if (!cond.eligible) {
                continue;
            }
        }
        ScheduledEventCandidate candidate;
        candidate.scene_id = tmpl.id;
        candidate.priority = tmpl.cooldown_hours > 0.0f ? 1.0f : 2.0f;
        candidate.reason = tmpl.label;
        candidates.push_back(std::move(candidate));
    }
    return candidates;
}

bool EventScheduler::is_on_cooldown(std::string_view scene_id,
                                    const SchedulerContext& ctx) const {
    const auto it = ctx.cooldowns.find(std::string(scene_id));
    if (it == ctx.cooldowns.end()) {
        return false;
    }
    return it->second > ctx.current_hour;
}

void EventScheduler::set_cooldown(std::string_view scene_id, f32 hours,
                                  SchedulerContext& ctx) const {
    ctx.cooldowns[std::string(scene_id)] = ctx.current_hour + hours;
}

const SceneTemplate* EventScheduler::find_template(std::string_view scene_id) const {
    const auto it = templates_.find(std::string(scene_id));
    if (it == templates_.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace eve::scene
