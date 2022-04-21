//
// Created by rebulien on 4/2/22.
//

#include "UMLClassifier.h"

void UMLClassifier::setName(std::string name) {
    Element::setName(std::move(name));
}

const bool &UMLClassifier::isUserDefined() const {
    return isUserDefined_;
}

bool &UMLClassifier::isUserDefined() {
    return isUserDefined_;
}

const bool &UMLClassifier::isAbstract() const {
    return isAbstract_;
}

bool &UMLClassifier::isAbstract() {
    return isAbstract_;
}
