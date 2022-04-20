//
// Created by rebulien on 4/2/22.
//

#ifndef DIAGRAMEDITOR_UMLCLASSIFIER_H
#define DIAGRAMEDITOR_UMLCLASSIFIER_H

#include "Element.h"
#include <string>
#include <utility>
#include <vector>

class UMLClassifier : public Element {
    bool _isUserDefined;
public:
    UMLClassifier() =delete;
    explicit UMLClassifier(std::string name) : Element{std::move(name)}, _isUserDefined{false} { };
    explicit UMLClassifier(std::string name, bool isUserDefined) : Element{std::move(name)}, _isUserDefined{isUserDefined} { }
    bool isUserDefined();
    void setName(std::string name);
};


#endif //DIAGRAMEDITOR_UMLCLASSIFIER_H
