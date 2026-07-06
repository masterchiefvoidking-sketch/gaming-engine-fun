#include <eve/ai/romance_hooks.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::ai {

bool RomanceSimulationHooks::load_reactions(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            RomanceReactionDefinition reaction;
            reaction.id = entry.value("id", "");
            reaction.dialogue_line = entry.value("dialogue", "");
            reaction.trust_delta = entry.value("trust_delta", 0.0f);
            reaction.affection_delta = entry.value("affection_delta", 0.0f);
            reaction.mood_shift = entry.value("mood_shift", 0.0f);
            reaction.min_trust = entry.value("min_trust", 0.0f);
            reaction.memory_tag = entry.value("memory_tag", "");
            register_reaction(std::move(reaction));
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void RomanceSimulationHooks::register_reaction(RomanceReactionDefinition reaction) {
    reactions_[reaction.id] = std::move(reaction);
}

RomanceReactionResult RomanceSimulationHooks::trigger(std::string_view reaction_id, RomanceContext& context,
                                                      RelationshipRegistry& relationships) const {
    RomanceReactionResult result;
    const auto it = reactions_.find(std::string(reaction_id));
    if (it == reactions_.end()) {
        result.reason = "Reaction not found";
        return result;
    }
    if (!respects_boundaries(reaction_id, context)) {
        result.reason = "Boundary respected - rejected";
        return result;
    }
    if (!context.consent_granted) {
        result.reason = "Consent not granted";
        return result;
    }
    if (context.trust < it->second.min_trust) {
        result.reason = "Insufficient trust";
        return result;
    }

    RelationshipModel& relationship = relationships.get_or_create(context.initiator, context.target);
    relationship.adjust_scores(it->second.trust_delta, it->second.affection_delta, 0.0f, 0.0f);
    if (!it->second.memory_tag.empty()) {
        relationship.add_memory(
            {it->second.memory_tag, it->second.dialogue_line, std::to_string(context.target), 0.5f, 0, {}});
        result.memory_created = it->second.memory_tag;
    }

    context.trust += it->second.trust_delta;
    context.affection += it->second.affection_delta;
    result.accepted = true;
    result.dialogue = it->second.dialogue_line;
    result.trust_delta = it->second.trust_delta;
    result.affection_delta = it->second.affection_delta;
    return result;
}

bool RomanceSimulationHooks::respects_boundaries(std::string_view reaction_id,
                                                 const RomanceContext& context) const {
    (void)reaction_id;
    for (const std::string& boundary : context.boundaries) {
        if (boundary == "no_flirting" && reaction_id == "flirt_compliment") {
            return false;
        }
    }
    return true;
}

} // namespace eve::ai
