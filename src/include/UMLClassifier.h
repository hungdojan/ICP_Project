//
// Created by rebulien on 4/2/22.
//

#ifndef DIAGRAMEDITOR_UMLCLASSIFIER_H
#define DIAGRAMEDITOR_UMLCLASSIFIER_H

#include "Element.h"
#include <string>
#include <utility>
#include <vector>

class UMLAttribute;

class UMLClassifier : public Element {
protected:
    bool isUserDefined_;
    bool isAbstract_{false};
public:
    UMLClassifier() =delete;
    explicit UMLClassifier(std::string name) : Element{std::move(name)}, isUserDefined_{false} { };
    explicit UMLClassifier(std::string name, bool isUserDefined) : Element{std::move(name)}, isUserDefined_{isUserDefined} { }
    const bool &isUserDefined() const;
    const bool &isAbstract() const;
    static UMLAttribute *createAttribute(bool isOperation, const std::string &name, UMLClassifier *type, const std::vector<UMLAttribute> &params);

    virtual bool &isAbstract();
    void setName(std::string name);
};


#endif //DIAGRAMEDITOR_UMLCLASSIFIER_H
