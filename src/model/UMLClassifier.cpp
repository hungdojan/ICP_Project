//
// Created by rebulien on 4/2/22.
//

#include "UMLClassifier.h"
#include "UMLAttribute.h"
#include "UMLOperation.h"
#include <stdexcept>

void UMLClassifier::setName(std::string name) {
    Element::setName(name);
}

const bool &UMLClassifier::isUserDefined() const {
    return isUserDefined_;
}

const bool &UMLClassifier::isAbstract() const {
    return isAbstract_;
}

bool &UMLClassifier::isAbstract() {
    return isAbstract_;
}

UMLAttribute *UMLClassifier::createAttribute(bool isOperation, const std::string &name, UMLClassifier *type,
                                             const std::vector<UMLAttribute> &params) {
    if (!isOperation && !params.empty())
        throw std::invalid_argument("Attribute instance cannot have parameters");
    if (isOperation)
        return new UMLOperation(name, type, params);
    return new UMLAttribute(name, type);
}