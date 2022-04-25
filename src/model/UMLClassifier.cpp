/**
 * @brief Definition of UMLClassifier class.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLClassifier.cpp
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "UMLClassifier.h"
#include "UMLAttribute.h"
#include "UMLOperation.h"
#include "UMLRelation.h"
#include <stdexcept>
#include <algorithm>
#include <QString>

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
    if (isOperation)
        return new UMLOperation(name, type, params);
    return new UMLAttribute(name, type);
}

const std::unordered_set<UMLClassifier *> &UMLClassifier::parentClasses() const {
    return parentClasses_;
}

const std::unordered_set<UMLRelation *> &UMLClassifier::relations() const {
    return relations_;
}

bool UMLClassifier::addParentClass(UMLClassifier *parentClass) {
    if (parentClass == nullptr)
        return false;
    // return insertion result
    auto result = parentClasses_.insert(parentClass);
    return result.second;
}

bool UMLClassifier::containsParentClass(UMLClassifier *parentClass) const {
    auto iter{std::find(parentClasses_.begin(), parentClasses_.end(), parentClass)};
    return iter != parentClasses_.end();
}

bool UMLClassifier::removeParentClass(UMLClassifier *parentClass) {
    if (parentClass == nullptr)
        return false;

    auto iter{std::find(parentClasses_.begin(), parentClasses_.end(), parentClass)};
    if (iter == parentClasses_.end())
        return false;
    parentClasses_.erase(iter);
    return true;
}

UMLClassifier *UMLClassifier::removeParentClass(const std::string &name) {
    auto iter{std::find_if(parentClasses_.begin(), parentClasses_.end(),
                           [name](UMLClassifier *c) { return c->name() == name; })};
    if (iter == parentClasses_.end())
        return nullptr;
    parentClasses_.erase(iter);
    return *iter;
}

UMLRelation *UMLClassifier::addRelation(UMLClassifier *dst) {
    throw std::invalid_argument("Cannot use function addRelation with UMLClassifier");
}

bool UMLClassifier::inRelationWith(UMLClassifier *dst) const {
    auto iter{std::find_if(relations_.begin(), relations_.end(),
                           [&,dst](UMLRelation *r) { return r->compareClassesInRelation(this, dst); })};
    return iter != relations_.end();
}

UMLRelation *UMLClassifier::getRelationWith(UMLClassifier *dst) {
    auto iter{std::find_if(relations_.begin(), relations_.end(),
                           [&,dst](UMLRelation *r) { return r->compareClassesInRelation(this, dst); })};
    if (iter == relations_.end())
        return nullptr;
    return *iter;
}

bool UMLClassifier::removeRelation(UMLClassifier *dstClass) {
    throw std::invalid_argument("Cannot use function removeRelation with UMLClassifier");
}

bool UMLClassifier::removeRelation(UMLRelation *relation) {
    throw std::invalid_argument("Cannot use function removeRelation with UMLClassifier");
}

void UMLClassifier::clearRelations() {
    // removes dependency of each relation, clear up resources
    // and clear whole collection afterwards
    for (auto r : relations_) {
        r->removeRelationDependency(this);
        delete r;
    }
    relations_.clear();
}

UMLClassifier::~UMLClassifier() {
    for (auto r : relations_) {
        r->removeRelationDependency(this);
        delete r;
    }
}

void UMLClassifier::createJsonObject(QJsonObject &object) {
    object.insert("_class", "UMLClassifier");
    object.insert("name", QString::fromStdString(name_));
    object.insert("isUserDefined", isUserDefined_);
    object.insert("isAbstract", isAbstract_);
}

std::unordered_set<UMLOperation *> UMLClassifier::getOperations() const {
    return {};
}
