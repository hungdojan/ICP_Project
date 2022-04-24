//
// Created by rebulien on 4/19/22.
//

#ifndef DIAGRAMEDITOR_SEQUENCEDIAGRAM_H
#define DIAGRAMEDITOR_SEQUENCEDIAGRAM_H
#include <utility>
#include <vector>
#include "Element.h"
#include "UMLClass.h"
#include "UMLObject.h"

class SequenceDiagram : public Element {
    std::vector<UMLObject *> objects_;

public:
    SequenceDiagram() =delete;
    explicit SequenceDiagram(const std::string &name) : Element{name} { }
    void addObject(UMLClass *instanceOfClass, const std::string &objName);
    UMLObject *getObject(const std::string &name);
    void removeObject(UMLObject *object);
    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    virtual void createObject(QJsonObject &object) { /* TODO: */ };
    ~SequenceDiagram() =default;
};


#endif //DIAGRAMEDITOR_SEQUENCEDIAGRAM_H
