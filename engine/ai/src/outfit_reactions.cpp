#include <algorithm>
#include <eve/ai/outfit_reactions.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::ai {

bool OutfitReactionSystem::load_from_file(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& entry : json) {
            OutfitReaction reaction;
            reaction.id = entry.value("id", "");
            reaction.outfit_id = entry.value("outfit_id", "");
            reaction.dialogue_line = entry.value("line", "");
            reaction.memory_tag = entry.value("memory_tag", "");
            reaction.affection_delta = entry.value("affection_delta", 0.0f);
            reaction.confidence_delta = entry.value("confidence_delta", 0.0f);
            if (entry.contains("outfit_tags")) {
                for (const auto& t : entry["outfit_tags"]) {
                    reaction.outfit_tags.push_back(t.get<std::string>());
                }
            }
            register_reaction(std::move(reaction));
        }
        return !reactions_.empty();
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

void OutfitReactionSystem::register_reaction(OutfitReaction reaction) {
    reactions_.push_back(std::move(reaction));
}

const OutfitReaction* OutfitReactionSystem::find_reaction(
    std::string_view outfit_id, const std::vector<std::string>& outfit_tags,
    RelationshipStage stage) const {
    for (const OutfitReaction& reaction : reactions_) {
        if (minimum_stage_level(stage) < minimum_stage_level(reaction.min_stage)) {
            continue;
        }
        if (!reaction.outfit_id.empty() && reaction.outfit_id == outfit_id) {
            return &reaction;
        }
        for (const std::string& tag : reaction.outfit_tags) {
            for (const std::string& worn : outfit_tags) {
                if (tag == worn) {
                    return &reaction;
                }
            }
        }
    }
    return nullptr;
}

bool OutfitReactionSystem::apply_reaction(const OutfitReaction& reaction,
                                          CharacterSimState& character) const {
    character.affection = std::clamp(character.affection + reaction.affection_delta, 0.0f, 1.0f);
    character.confidence =
        std::clamp(character.confidence + reaction.confidence_delta, 0.0f, 1.0f);
    character.record_interaction(reaction.id);
    return true;
}

} // namespace eve::ai
