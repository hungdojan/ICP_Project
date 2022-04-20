//
// Created by rebulien on 4/20/22.
//

#include "UMLOperation.h"
#include "UMLAttribute.h"
#include "UMLClassifier.h"
#include <algorithm>
#include <sstream>

UMLOperation::UMLOperation(const std::string &name, UMLClassifier *type, std::vector<UMLAttribute> parameters)
    : UMLAttribute(name, type) {
    for (const auto &item : parameters) {
        addParameter(item);
    }
}

const std::vector<UMLAttribute> &UMLOperation::parameters() const {
    return parameters_;
}

bool UMLOperation::addParameter(const UMLAttribute &parameter) {
    auto iter{std::find_if(parameters_.begin(), parameters_.end(),
                           [parameter](const UMLAttribute &attr) { return attr.name() == parameter.name(); })};

    if (iter != parameters_.end())
        return false;
    parameters_.push_back(parameter);
    return true;
}

bool UMLOperation::addOrReplaceParameter(const UMLAttribute &parameter, UMLAttribute *oldInstance) {
    auto iter{std::find_if(parameters_.begin(), parameters_.end(),
                           [parameter](const UMLAttribute &attr) { return attr.name() == parameter.name(); })};
    bool result = false;
    if (iter != parameters_.end()) {
        *oldInstance = *iter;
        parameters_.erase(iter);
        result = true;
    }
    parameters_.push_back(parameter);
    return result;
}

bool UMLOperation::getParameterByName(const std::string &name, UMLAttribute *found) {
    auto iter{std::find_if(parameters_.begin(), parameters_.end(),
                           [name](const UMLAttribute &attr) { return attr.name() == name; })};
    if (iter == parameters_.end())
        return false;

    *found = *iter;
    return true;
}

bool UMLOperation::removeParameter(const std::string &name, UMLAttribute *removed) {
    auto iter{std::find_if(parameters_.begin(), parameters_.end(),
                           [name](const UMLAttribute &attr) { return attr.name() == name; })};
    if (iter == parameters_.end())
        return false;
    *removed = *iter;
    parameters_.erase(iter);
    return true;
}

bool UMLOperation::removeParameter(const UMLAttribute &parameter) {
    // TODO: check if std::find works otherwise return by reference is needed to prevent from leakage
    // FIXME: need to define operator==
    // auto iter{std::find(parameters_.begin(), parameters_.end(),parameter)};
    auto iter{std::find_if(parameters_.begin(), parameters_.end(),
                           [parameter](const UMLAttribute &attr) { return attr.name() == parameter.name() &&
                                                                          attr.type() == parameter.type(); })};
    if (iter == parameters_.end())
        return false;
    parameters_.erase(iter);
    return true;
}

void UMLOperation::clearParameters(std::vector<UMLAttribute> *vectorContent) {
    if (vectorContent != nullptr)
        *vectorContent = std::move(parameters_);
    parameters_.clear();
}

UMLOperation::operator std::string() const {
    std::ostringstream stream;
    if (visibility_ != '\0')
        stream << visibility_ << " ";
    stream << type_->name() << " " << name_ << "(";
    for (auto i = parameters_.begin(); i != parameters_.end(); ++i) {
        if (i != parameters_.begin())
            stream << " ";
        stream << *i;
    }
    stream << ")";
    return UMLAttribute::operator std::string();
}

std::ostream &operator<<(std::ostream &strm, const UMLOperation &operation) {
    std::string str{operation};
    return strm << str;
}