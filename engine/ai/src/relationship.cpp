#include <algorithm>
#include <eve/ai/relationship.hpp>

namespace eve::ai {

void RelationshipModel::record_conversation(std::string_view line) {
    conversation_history.emplace_back(line);
    if (conversation_history.size() > 100) {
        conversation_history.erase(conversation_history.begin());
    }
}

void RelationshipModel::add_shared_experience(std::string_view experience) {
    shared_experiences.emplace_back(experience);
}

void RelationshipModel::add_memory(MemoryEntry memory) {
    shared_memories.push_back(std::move(memory));
}

void RelationshipModel::adjust_scores(f32 trust_delta, f32 affection_delta, f32 respect_delta,
                                      f32 humor_delta) {
    auto clamp = [](f32 value) { return std::clamp(value, 0.0f, 1.0f); };
    scores.trust = clamp(scores.trust + trust_delta);
    scores.affection = clamp(scores.affection + affection_delta);
    scores.respect = clamp(scores.respect + respect_delta);
    scores.humor = clamp(scores.humor + humor_delta);
}

std::string RelationshipRegistry::make_key(EntityId a, EntityId b) {
    if (a > b) {
        std::swap(a, b);
    }
    return std::to_string(a) + ":" + std::to_string(b);
}

RelationshipModel& RelationshipRegistry::get_or_create(EntityId a, EntityId b) {
    const std::string key = make_key(a, b);
    auto it = relationships_.find(key);
    if (it == relationships_.end()) {
        RelationshipModel model;
        model.character_a = std::min(a, b);
        model.character_b = std::max(a, b);
        it = relationships_.emplace(key, std::move(model)).first;
    }
    return it->second;
}

const RelationshipModel* RelationshipRegistry::find(EntityId a, EntityId b) const {
    const auto it = relationships_.find(make_key(a, b));
    if (it == relationships_.end()) {
        return nullptr;
    }
    return &it->second;
}

} // namespace eve::ai
