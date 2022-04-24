#ifndef DIAGRAMEDITOR_UMLOBJECT_H
#define DIAGRAMEDITOR_UMLOBJECT_H
#include <string>
#include "Element.h"

class UMLObject : public Element {
    UMLClass *model_;
public:
    UMLObject() =delete;
    explicit UMLObject(const std::string &name, UMLClass *instanceOfClass) : Element{name}, model_{instanceOfClass} { }
    const UMLClass *model() const;
    void setModel(UMLClass *model);
    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    virtual void createObject(QJsonObject &object) { /* TODO: */ };
};

#endif //DIAGRAMEDITOR_UMLOBJECT_H