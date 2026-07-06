#include <eve/editor/relationship_inspector.hpp>
#include <sstream>

namespace eve::editor {

std::string RelationshipInspector::summarize(const ai::RelationshipModel& relationship) const {
    std::ostringstream out;
    out << "Trust: " << relationship.scores.trust
        << ", Affection: " << relationship.scores.affection
        << ", Respect: " << relationship.scores.respect << ", Humor: " << relationship.scores.humor;
    return out.str();
}

void RelationshipInspector::render_scores(const ai::RelationshipScores& scores) const {
    (void)scores;
}

} // namespace eve::editor
