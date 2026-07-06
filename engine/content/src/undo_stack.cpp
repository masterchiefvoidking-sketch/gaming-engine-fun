#include <eve/content/undo_stack.hpp>

namespace eve::content {

void UndoStack::push(UndoCommand command) {
    undo_stack_.push_back(std::move(command));
    redo_stack_.clear();
}

bool UndoStack::undo() {
    if (undo_stack_.empty()) {
        return false;
    }
    UndoCommand command = std::move(undo_stack_.back());
    undo_stack_.pop_back();
    if (command.undo) {
        command.undo();
    }
    redo_stack_.push_back(std::move(command));
    return true;
}

bool UndoStack::redo() {
    if (redo_stack_.empty()) {
        return false;
    }
    UndoCommand command = std::move(redo_stack_.back());
    redo_stack_.pop_back();
    if (command.redo) {
        command.redo();
    }
    undo_stack_.push_back(std::move(command));
    return true;
}

void UndoStack::clear() {
    undo_stack_.clear();
    redo_stack_.clear();
}

} // namespace eve::content
