/**
 * @brief Declaration of Element class
 * Element is base class for (almost) all elements within ClassDiagram.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22
 *
 * @file Element.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_ELEMENT_H
#define DIAGRAMEDITOR_ELEMENT_H
#include <string>
#include <utility>
#include <QJsonObject>

class Element {
protected:
    /** Element's name */
    std::string name_;
public:
    Element() =delete;
    /**
     * @brief Class constructor.
     * @param name Element's name.
     */
    explicit Element(std::string name) : name_{std::move(name)} { };

    /**
     * @brief Returns read-only reference to element's name.
     * @return Read-only reference of element's name.
     */
    const std::string &name() const;

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    virtual void createJsonObject(QJsonObject &object) =0;

    /**
     * @brief Class destructor.
     */
    virtual ~Element() =default;

protected:
    /**
     * @brief Updates element's name.
     * @param newName New element's name.
     */
    virtual void setName(const std::string &newName);
};


#endif //DIAGRAMEDITOR_ELEMENT_H
