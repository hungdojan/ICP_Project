/**
 * @brief Declaration of abstract class ISubject.
 * Subject is the first part of Observer design pattern. Classes that derives from this abstract class
 * must implement 3 methods: attach, detach and notify. That allows subscribers to update themselves
 * when any event happens.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file ISubject.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_ISUBJECT_H
#define DIAGRAMEDITOR_ISUBJECT_H
#include "IObserver.h"

/**
 * @brief Representation of subject in design pattern.
 */
class ISubject {
protected:
    /** Set of observers */
    std::unordered_set<IObserver *> observers_;
public:
    /**
     * @brief Adds observer into the collection of observers.
     * @param observer Instance of observer.
     */
    virtual void attach(IObserver *observer) =0;

    /**
     * @brief Removes observer from the collection of observers.
     * @param observer Instance of observer.
     */
    virtual void detach(IObserver *observer) =0;

    /**
     * @brief Sends message to all subscribers/observers.
     * @param msg Message content.
     */
    virtual void notify(const std::string &msg) =0;

    /**
     * Class destructor.
     */
    virtual ~ISubject() =default;
};

#endif //DIAGRAMEDITOR_ISUBJECT_H
