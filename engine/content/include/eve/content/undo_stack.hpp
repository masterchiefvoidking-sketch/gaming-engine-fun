#pragma once

#include <functional>
#include <string>
#include <vector>

namespace eve::content {

struct UndoCommand {
    std::string label;
    std::function<void()> undo;
    std::function<void()> redo;
};

class UndoStack {
public:
    void push(UndoCommand command);
    bool undo();
    bool redo();
    void clear();

    [[nodiscard]] bool can_undo() const { return !undo_stack_.empty(); }
    [[nodiscard]] bool can_redo() const { return !redo_stack_.empty(); }
    [[nodiscard]] std::size_t undo_depth() const { return undo_stack_.size(); }

private:
    std::vector<UndoCommand> undo_stack_;
    std::vector<UndoCommand> redo_stack_;
};

} // namespace eve::content
