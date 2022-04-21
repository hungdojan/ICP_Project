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
};

#endif //DIAGRAMEDITOR_UMLOBJECT_H