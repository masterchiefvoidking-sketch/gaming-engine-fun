#pragma once

#include <eve/ai/relationship.hpp>
#include <eve/ai/schedule.hpp>

namespace eve::editor {

class RelationshipInspector {
public:
    [[nodiscard]] std::string summarize(const ai::RelationshipModel& relationship) const;
    void render_scores(const ai::RelationshipScores& scores) const;
};

} // namespace eve::editor
