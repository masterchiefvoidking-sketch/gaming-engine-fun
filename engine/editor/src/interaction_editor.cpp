#include <eve/editor/interaction_editor.hpp>
#include <sstream>

namespace eve::editor {

void InteractionEditor::load_definition(const world::InteractionDefinition& definition) {
    definition_ = definition;
}

bool InteractionEditor::add_action(const world::ContextAction& action) {
    definition_.actions.push_back(action);
    return true;
}

std::string InteractionEditor::export_json() const {
    std::ostringstream out;
    out << "{\"id\":\"" << definition_.id << "\",\"actions\":" << definition_.actions.size() << "}";
    return out.str();
}

} // namespace eve::editor
