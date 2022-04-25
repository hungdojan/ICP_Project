#include "UMLMessage.h"
#include "UMLClass.h"
#include <algorithm>

const UMLObject &UMLMessage::dst() const {
    return dst_;
}

const UMLObject &UMLMessage::src() const {
    return src_;
}

const UMLOperation *UMLMessage::baseOperation() const {
    return baseOperation_;
}

bool UMLMessage::setOperation(const std::string &name) {
    auto operationsAvailable = src_.model()->getOperations();
    auto iter{std::find_if(operationsAvailable.begin(), operationsAvailable.end(),
                           [name](UMLOperation *o) { return o->name() == name; })};
    if (iter == operationsAvailable.end())
        return false;
    baseOperation_ = *iter;
    return true;
}

bool UMLMessage::setOperation(UMLOperation *operation) {
    auto operationsAvailable = src_.model()->getOperations();
    auto iter{operationsAvailable.find(operation)};
    if (iter == operationsAvailable.end())
        return false;
    baseOperation_ = *iter;
    return true;
}
