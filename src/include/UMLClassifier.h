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
protected:
    bool isUserDefined_;
    bool isAbstract_{false};
public:
    UMLClassifier() =delete;
    explicit UMLClassifier(std::string name) : Element{std::move(name)}, isUserDefined_{false} { };
    explicit UMLClassifier(std::string name, bool isUserDefined) : Element{std::move(name)}, isUserDefined_{isUserDefined} { }
    const bool &isUserDefined() const;
    bool &isUserDefined();
    const bool &isAbstract() const;

    virtual bool &isAbstract();
    void setName(std::string name);
};


#endif //DIAGRAMEDITOR_UMLCLASSIFIER_H
