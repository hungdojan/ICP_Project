/**
 * @brief Declaration of UMLObject class.
 * UMLObject represents abstract structure of object in sequence diagram. Object has a reference to a class it is instantiated from.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLObject.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_UMLOBJECT_H
#define DIAGRAMEDITOR_UMLOBJECT_H
#include <string>
#include "Element.h"
#include "UMLClass.h"

/**
 * @brief Representation of object in sequence diagram.
 */
class UMLObject : public Element, public IObserver {
    /** Base class model of object. */
    UMLClass *model_;
public:
    UMLObject() =delete;
    /**
     * @brief Class constructor.
     * @param name Object's name.
     * @param instanceOfClass Object's class model.
     */
    explicit UMLObject(const std::string &name, UMLClass *instanceOfClass) : Element{name}, model_{instanceOfClass} { }

    /**
     * @brief Returns object's model.
     * @return Instance of object's class model.
     */
    UMLClass *model() const;

    /**
     * @brief Updates object's class model.
     * @param model New class model.
     */
    void setModel(UMLClass *model);

    /**
     * @brief Message receiver method.
     * @param msg Message content.
     */
    void update(const std::string &msg) override;

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
   void createJsonObject(QJsonObject &object) override;
};

#endif //DIAGRAMEDITOR_UMLOBJECT_H