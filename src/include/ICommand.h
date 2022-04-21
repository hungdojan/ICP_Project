//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_ICOMMAND_H
#define DIAGRAMEDITOR_ICOMMAND_H

class ICommand {
public:
    ~ICommand() =default;
    virtual void execute() =0;
    virtual void undo()    =0;
    virtual void redo()    =0;
};

#endif //DIAGRAMEDITOR_ICOMMAND_H
