/**
 * @brief Declaration of CommandBuilder class.
 * CommandBuilder holds all undo and redo commands that can be called.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file CommandBuilder.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_COMMANDBUILDER_H
#define DIAGRAMEDITOR_COMMANDBUILDER_H
#include <vector>
#include <memory>
#include <QObject>
#include "ICommand.h"

class CommandBuilder : public QObject {
Q_OBJECT
    CommandBuilder() =default;
public:
    /** Stack of undo commands */
    std::vector<std::shared_ptr<ICommand>> undoStack;
    /** Stack of redo commands */
    std::vector<std::shared_ptr<ICommand>> redoStack;
    ~CommandBuilder() override =default;
    static CommandBuilder &get_commander() {
        static CommandBuilder commandBuilder;
        return commandBuilder;
    }
    /** Execute command */
    void execute(const std::shared_ptr<ICommand>& cmd);
    /** Undo command */
    void undo();
    /** Redo command */
    void redo();
    /** Clear stacks */
    void clear();
public slots:
    void redoSlot() { redo(); };
    void undoSlot() { undo(); };
};
#endif //DIAGRAMEDITOR_COMMANDBUILDER_H
