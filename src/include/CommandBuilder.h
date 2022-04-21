//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_COMMANDBUILDER_H
#define DIAGRAMEDITOR_COMMANDBUILDER_H
#include <stack>
#include "ICommand.h"

class CommandBuilder {
    std::stack<ICommand> undoStack;
    std::stack<ICommand> redoStack;
public:
    ~CommandBuilder() =default;
    void execute(const ICommand &cmd);
    void undo();
    void redo();
};
#endif //DIAGRAMEDITOR_COMMANDBUILDER_H
