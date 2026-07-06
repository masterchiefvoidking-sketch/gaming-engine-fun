#include <algorithm>
#include <eve/ai/relationship_stages.hpp>
#include <eve/scene/scene_conditions.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::scene {

namespace {

ai::RelationshipStage parse_stage(const std::string& value) {
    if (value == "acquaintance") return ai::RelationshipStage::Acquaintance;
    if (value == "friend") return ai::RelationshipStage::Friend;
    if (value == "close_friend") return ai::RelationshipStage::CloseFriend;
    if (value == "romantic_interest") return ai::RelationshipStage::RomanticInterest;
    if (value == "partner") return ai::RelationshipStage::Partner;
    return ai::RelationshipStage::Stranger;
}

ai::Mood parse_mood(const std::string& value) {
    if (value == "happy") return ai::Mood::Happy;
    if (value == "sad") return ai::Mood::Sad;
    if (value == "anxious") return ai::Mood::Anxious;
    if (value == "romantic") return ai::Mood::Romantic;
    if (value == "playful") return ai::Mood::Playful;
    if (value == "tired") return ai::Mood::Tired;
    if (value == "stressed") return ai::Mood::Stressed;
    return ai::Mood::Neutral;
}

void load_string_array(const nlohmann::json& json, std::string_view key,
                       std::vector<std::string>& out) {
    if (!json.contains(key)) {
        return;
    }
    for (const auto& item : json[key]) {
        out.push_back(item.get<std::string>());
    }
}

} // namespace

bool SceneConditionEvaluator::load_conditions(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            EventConditionDefinition cond;
            cond.id = entry.value("id", "");
            cond.label = entry.value("label", "");
            cond.min_stage = parse_stage(entry.value("min_stage", "stranger"));
            cond.mood_required = entry.contains("required_mood");
            if (cond.mood_required) {
                cond.required_mood = parse_mood(entry.value("required_mood", "neutral"));
            }
            cond.min_trust = entry.value("min_trust", 0.0f);
            cond.min_affection = entry.value("min_affection", 0.0f);
            cond.min_comfort = entry.value("min_comfort", 0.0f);
            cond.min_energy = entry.value("min_energy", 0.0f);
            cond.max_stress = entry.value("max_stress", 1.0f);
            cond.require_consent = entry.value("require_consent", true);
            cond.require_privacy = entry.value("require_privacy", false);
            load_string_array(entry, "rooms", cond.required_rooms);
            load_string_array(entry, "outfits", cond.required_outfits);
            load_string_array(entry, "memories", cond.required_memories);
            load_string_array(entry, "blocked_memories", cond.blocked_memories);
            load_string_array(entry, "time_of_day", cond.time_of_day);
            load_string_array(entry, "weather", cond.weather);
            load_string_array(entry, "story_flags", cond.story_flags);
            register_condition(std::move(cond));
        }
        return !conditions_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void SceneConditionEvaluator::register_condition(EventConditionDefinition condition) {
    conditions_[condition.id] = std::move(condition);
}

ConditionEvaluationResult SceneConditionEvaluator::evaluate(std::string_view condition_id,
                                                            const SceneRuntimeContext& ctx) const {
    const auto it = conditions_.find(std::string(condition_id));
    if (it == conditions_.end()) {
        ConditionEvaluationResult result;
        result.rejection_reason = "Condition not found";
        return result;
    }
    return evaluate_definition(it->second, ctx);
}

ConditionEvaluationResult SceneConditionEvaluator::evaluate_definition(
    const EventConditionDefinition& cond, const SceneRuntimeContext& ctx) const {
    ConditionEvaluationResult result;

    if (ai::minimum_stage_level(ctx.stage) < ai::minimum_stage_level(cond.min_stage)) {
        result.rejection_reason = "Relationship stage too low";
        return result;
    }
    if (ctx.trust < cond.min_trust) {
        result.rejection_reason = "Trust too low";
        return result;
    }
    if (ctx.affection < cond.min_affection) {
        result.rejection_reason = "Affection too low";
        return result;
    }
    if (ctx.comfort < cond.min_comfort) {
        result.rejection_reason = "Comfort too low";
        return result;
    }
    if (ctx.energy < cond.min_energy) {
        result.rejection_reason = "Energy too low";
        return result;
    }
    if (ctx.stress > cond.max_stress) {
        result.rejection_reason = "Stress too high";
        return result;
    }
    if (cond.mood_required && ctx.mood != cond.required_mood) {
        result.rejection_reason = "Mood requirement not met";
        return result;
    }
    if (cond.require_consent && !ctx.consent_granted) {
        result.rejection_reason = "Consent not granted";
        return result;
    }
    if (cond.require_privacy && !ctx.privacy_ok) {
        result.rejection_reason = "Privacy requirement not met";
        return result;
    }

    if (!cond.required_rooms.empty()) {
        bool room_ok = false;
        for (const std::string& room : cond.required_rooms) {
            if (room == ctx.active_room) {
                room_ok = true;
                break;
            }
        }
        if (!room_ok) {
            result.rejection_reason = "Wrong room";
            return result;
        }
    }

    if (!cond.required_outfits.empty()) {
        bool outfit_ok = false;
        for (const std::string& outfit : cond.required_outfits) {
            if (outfit == ctx.active_outfit) {
                outfit_ok = true;
                break;
            }
        }
        if (!outfit_ok) {
            result.rejection_reason = "Wrong outfit";
            return result;
        }
    }

    if (!cond.weather.empty()) {
        bool weather_ok = false;
        for (const std::string& w : cond.weather) {
            if (w == ctx.weather) {
                weather_ok = true;
                break;
            }
        }
        if (!weather_ok) {
            result.rejection_reason = "Weather requirement not met";
            return result;
        }
    }

    if (!cond.time_of_day.empty()) {
        bool time_ok = false;
        for (const std::string& bucket : cond.time_of_day) {
            if (time_matches(ctx.time_of_day, bucket)) {
                time_ok = true;
                break;
            }
        }
        if (!time_ok) {
            result.rejection_reason = "Time requirement not met";
            return result;
        }
    }

    for (const std::string& mem : cond.required_memories) {
        if (std::find(ctx.known_memories.begin(), ctx.known_memories.end(), mem) ==
            ctx.known_memories.end()) {
            result.rejection_reason = "Required memory missing: " + mem;
            return result;
        }
    }

    for (const std::string& mem : cond.blocked_memories) {
        if (std::find(ctx.known_memories.begin(), ctx.known_memories.end(), mem) !=
            ctx.known_memories.end()) {
            result.rejection_reason = "Blocked by memory: " + mem;
            return result;
        }
    }

    for (const std::string& flag : cond.story_flags) {
        if (std::find(ctx.story_flags.begin(), ctx.story_flags.end(), flag) ==
            ctx.story_flags.end()) {
            result.rejection_reason = "Story flag missing: " + flag;
            return result;
        }
    }

    for (const std::string& completed : ctx.completed_events) {
        if (completed == cond.id) {
            result.rejection_reason = "Event already completed";
            return result;
        }
    }

    result.eligible = true;
    return result;
}

bool SceneConditionEvaluator::time_matches(f32 hour, std::string_view bucket) const {
    if (bucket == "morning") return hour >= 6.0f && hour < 12.0f;
    if (bucket == "afternoon") return hour >= 12.0f && hour < 17.0f;
    if (bucket == "evening") return hour >= 17.0f && hour < 21.0f;
    if (bucket == "night") return hour >= 21.0f || hour < 6.0f;
    return true;
}

} // namespace eve::scene
