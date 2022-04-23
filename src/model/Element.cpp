/**
 * @brief Definition of Element class
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22
 *
 * @file Element.cpp
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */
#include "Element.h"

const std::string &Element::name() const {
    return name_;
}

void Element::setName(const std::string &newName) {
    name_ = newName;
}
