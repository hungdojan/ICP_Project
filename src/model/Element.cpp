//
// Created by rebulien on 4/2/22.
//

#include "Element.h"

#include <utility>

const std::string &Element::name() const {
    return name_;
}

void Element::setName(const std::string &newName) {
    name_ = newName;
}
