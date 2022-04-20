//
// Created by rebulien on 4/2/22.
//

#include "UMLClassifier.h"

void UMLClassifier::setName(std::string name) {
    Element::setName(name);
}

bool UMLClassifier::isUserDefined() {
    return _isUserDefined;
}
