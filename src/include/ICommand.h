/**
 * @brief Declaration of abstract class ICommand.
 * This class represents design pattern Command. This design pattern helps with the implementation
 * of undo/redo actions in DiagramEditor.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22
 *
 * @file ICommand.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_ICOMMAND_H
#define DIAGRAMEDITOR_ICOMMAND_H

/**
 * @brief Representation of Command design pattern.
 */
class ICommand {
public:
    /**
     * @brief Execution method that is defined end-user.
     */
    virtual void execute() =0;

    /**
     * @brief User-defined undo action.
     */
    virtual void undo() =0;

    /**
     * @brief User-defined redo action.
     */
    virtual void redo() =0;

    /**
     * Class destructor.
     */
    ~ICommand() =default;
};

#endif //DIAGRAMEDITOR_ICOMMAND_H
