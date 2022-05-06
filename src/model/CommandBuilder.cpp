/**
 * @brief Definition of CommandBuilder functions.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file CommandBuilder.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */
#include "CommandBuilder.h"

void CommandBuilder::execute(const std::shared_ptr<ICommand>& cmd) {
    if (cmd == nullptr)
        return;

    undoStack.push_back(cmd);
    redoStack.clear();  // empty redo stack on execute
    cmd->execute();
}

void CommandBuilder::undo() {
    if (undoStack.empty())
        return;
    auto cmd = undoStack.back();
    cmd->undo();
    redoStack.push_back(cmd);
    undoStack.pop_back();
}

void CommandBuilder::redo() {
    if (redoStack.empty())
        return;
    auto cmd = redoStack.back();
    cmd->redo();
    redoStack.pop_back();
    undoStack.push_back(cmd);
}

void CommandBuilder::clear() {
    undoStack.clear();
    redoStack.clear();
}