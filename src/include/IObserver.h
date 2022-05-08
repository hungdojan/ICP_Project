/**
 * @brief Declaration of abstract class IObserver.
 * Observer is the second part of Observer design pattern. Observer is usually subscribed to
 * one or more Subjects and waits for a message from them. Observer implements _update_ function as
 * an answer to the message.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file IObserver.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_IOBSERVER_H
#define DIAGRAMEDITOR_IOBSERVER_H
#include <string>

/**
 * @brief Representation of subscriber in desing pattern.
 */
class IObserver {
public:
    /**
     * @brief Message receiver method.
     * @param msg Message content.
     */
    virtual void update(const std::string &msg) =0;

    /**
     * @brief Class destructor.
     */
    ~IObserver() =default;
};

#endif //DIAGRAMEDITOR_IOBSERVER_H
