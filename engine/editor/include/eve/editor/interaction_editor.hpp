#pragma once

#include <eve/world/interaction.hpp>
#include <string>

namespace eve::editor {

class InteractionEditor {
public:
    void load_definition(const world::InteractionDefinition& definition);
    bool add_action(const world::ContextAction& action);
    [[nodiscard]] const world::InteractionDefinition& definition() const { return definition_; }
    [[nodiscard]] std::string export_json() const;

private:
    world::InteractionDefinition definition_;
};

} // namespace eve::editor
