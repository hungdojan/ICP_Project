/**
 * @brief Definition of class UMLOperation methods.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLOperation.cpp
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "UMLOperation.h"
#include "UMLAttribute.h"
#include "UMLClassifier.h"
#include "ClassDiagram.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <QString>
#include <QJsonArray>

UMLOperation::UMLOperation(const std::string &name, UMLClassifier *type, const std::vector<UMLAttribute *> &parameters)
    : UMLAttribute(name, type) {
    type_->attach(this);
    for (auto item : parameters) {
        addParameter(item);
    }
}

const std::vector<UMLAttribute*> &UMLOperation::parameters() const {
    return parameters_;
}

bool UMLOperation::addParameter(UMLAttribute *parameter) {
    auto iter{std::find_if(parameters_.begin(), parameters_.end(),
                           [&parameter](UMLAttribute *attr) { return attr->name() == parameter->name(); })};

    if (iter != parameters_.end())
        return false;
    parameters_.push_back(parameter);
    return true;
}

UMLAttribute *UMLOperation::addOrReplaceParameter(UMLAttribute *parameter) {
    auto iter{std::find_if(parameters_.begin(), parameters_.end(),
                           [parameter](const UMLAttribute *attr) { return attr->name() == parameter->name(); })};
    UMLAttribute *found;
    if (iter != parameters_.end()) {
        found = *iter;
        parameters_.erase(iter);
    }
    parameters_.push_back(parameter);
    return found;
}

UMLAttribute *UMLOperation::getParameterByName(const std::string &name) const {
    auto iter{std::find_if(parameters_.begin(), parameters_.end(),
                           [name](const UMLAttribute *attr) { return attr->name() == name; })};
    if (iter == parameters_.end())
        return nullptr;

    return *iter;
}

bool UMLOperation::removeParameter(const std::string &name) {
    auto iter{std::find_if(parameters_.begin(), parameters_.end(),
                           [name](UMLAttribute *attr) { return attr->name() == name; })};
    if (iter == parameters_.end())
        return false;
    auto found = *iter;
    parameters_.erase(iter);
    delete found;
    return true;
}

bool UMLOperation::removeParameter(UMLAttribute *parameter) {
    auto iter{std::find(parameters_.begin(), parameters_.end(), parameter)};
    if (iter == parameters_.end())
        return false;
    parameters_.erase(iter);
    return true;
}

void UMLOperation::clearParameters(std::vector<UMLAttribute *> *vectorContent) {
    if (vectorContent != nullptr)
        *vectorContent = std::move(parameters_);
    parameters_.clear();
}

UMLOperation::operator std::string() const {
    std::ostringstream stream;
    if (visibility_ != 0)
        stream << visibility_;
    stream << type_->name() << " " << name_ << "(";
    for (auto i = parameters_.begin(); i != parameters_.end(); ++i) {
        if (i != parameters_.begin())
            stream << ", ";
        stream << **i;
    }
    stream << ")";
    return stream.str();
}

std::ostream &operator<<(std::ostream &strm, const UMLOperation &operation) {
    std::string str{operation};
    return strm << str;
}

UMLOperation::~UMLOperation() {
    for (auto p : parameters_) {
        delete p;
    }
    notify("DELETE");
}

void UMLOperation::createJsonObject(QJsonObject &object) {
    object.insert("_class", "UMLOperation");
    object.insert("name", QString::fromStdString(name_));
    object.insert("type", QString::fromStdString(type_->name()));
    object.insert("visibility", visibility_);
    QJsonArray paramsArray;
    for (auto param : parameters_) {
        QJsonObject qParam;
        param->createJsonObject(qParam);
        paramsArray.push_back(qParam);
    }
    object.insert("parameters", paramsArray);
}

void UMLOperation::attach(IObserver *observer) {
    observers_.insert(observer);
}

void UMLOperation::detach(IObserver *observer) {
    observers_.erase(observer);
}

void UMLOperation::notify(const std::string &msg) {
    for (auto o : observers_) {
        o->update(msg);
    }
}