//
// Created by rebulien on 4/19/22.
//

#ifndef DIAGRAMEDITOR_SEQUENCEDIAGRAM_H
#define DIAGRAMEDITOR_SEQUENCEDIAGRAM_H
#include <utility>
#include "Element.h"


class SequenceDiagram : public Element {
public:
    explicit SequenceDiagram(std::string name) : Element{std::move(name)} { }
};


#endif //DIAGRAMEDITOR_SEQUENCEDIAGRAM_H
