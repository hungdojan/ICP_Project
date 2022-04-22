//
// Created by rebulien on 4/5/22.
//

#include "UMLClass.h"
#include "UMLOperation.h"
#include <stdexcept>
#include <algorithm>

UMLClass::UMLClass(const std::string& name, const std::vector<UMLAttribute *>& attributes) : UMLClassifier{name} {
    for (auto attribute : attributes) {
        // TODO: call addAttribute function
        this->attributes_.push_back(attribute);
    }
}

const std::string &UMLClass::stereotype() const {
    return stereotype_;
}

std::string &UMLClass::stereotype() {
    return stereotype_;
}

const std::vector<UMLAttribute *> &UMLClass::attributes() const {
    return attributes_;
}

const std::unordered_set<UMLClassifier *> &UMLClass::parentClasses() const {
    return parentClasses_;
}

bool UMLClass::addAttribute(UMLAttribute *attr) {
    if (attr == nullptr)
        return false;
    // no collision in names between attributes AND names
    auto iter{std::find_if(attributes_.begin(), attributes_.end(),
                            [attr](UMLAttribute *a) { return a->name() == attr->name(); })};
    // when collision if names is detected, insertion is halted
    if (iter != attributes_.end())
        return false;
    attributes_.push_back(attr);
    return true;
}

UMLAttribute *UMLClass::addOrReplaceAttribute(UMLAttribute *attr) {
    if (attr == nullptr)
        throw std::invalid_argument("Inserting nullptr is forbidden");
    // no collision in names between attributes AND names
    auto iter{std::find_if(attributes_.begin(), attributes_.end(),
                            [attr](UMLAttribute *a) { return a->name() == attr->name(); })};
    UMLAttribute *found = nullptr;
    if (iter != attributes_.end()) {
        found = *iter;
        attributes_.erase(iter);
    }
    attributes_.push_back(attr);
    return found;
}

UMLAttribute *UMLClass::getAttribute(const std::string &name) {
    // search for attribute with a given name
    auto iter{ std::find_if(attributes_.begin(), attributes_.end(),
                            [name](UMLAttribute *a) { return a->name() == name; })};
    if (iter != attributes_.end())
        return *iter;
    return nullptr;
}

const std::list<UMLAttribute *> UMLClass::getOperations() const {
    // TODO: merge operations_ of this and
    return std::list<UMLAttribute *>();
}

UMLAttribute *UMLClass::getAttributeAtPosition(int pos) {
    if (pos < 0 || pos >= attributes_.size())
        return nullptr;
    auto iter{attributes_.begin()};
    std::advance(iter, pos);
    return *iter;
}

int UMLClass::getAttributesPosition(UMLAttribute *attr) {
    if (attr == nullptr)
        return -1;
    auto iter{std::find(attributes_.begin(), attributes_.end(), attr)};
    if (iter == attributes_.end())
        return -1;
    return std::distance(attributes_.begin(), iter);
}

bool UMLClass::moveAttributeToPosition(const std::string &name, int pos) {
    return false;
}

bool UMLClass::updateAttributeName(UMLAttribute *attribute, const std::string &newName) {
    if (attribute == nullptr)
        return false;

    auto iterAttr{std::find(attributes_.begin(), attributes_.end(), attribute)};
    if (iterAttr == attributes_.end())
        return false;

    UMLAttribute *attrByName{getAttribute(newName)};
    if (attrByName == nullptr)
        return false;
    attribute->setName(newName);
    return true;
}

bool UMLClass::updateAttributeName(std::string oldName, std::string newName) {
    UMLAttribute *oldAttr{getAttribute(oldName)};
    UMLAttribute *newAttr{getAttribute(newName)};
    if (oldAttr != nullptr && newAttr == nullptr) {
        oldAttr->setName(newName);
        return true;
    }
    return false;
}

UMLAttribute *UMLClass::removeAttribute(const std::string &name) {
    auto iter{std::find_if(attributes_.begin(), attributes_.end(),
                           [name](UMLAttribute *attr) { return name == attr->name(); })};
    if (iter == attributes_.end())
        return nullptr;
    UMLAttribute *found = *iter;
    attributes_.erase(iter);
    return found;
}

bool UMLClass::removeAttribute(UMLAttribute *attr) {
    if (attr == nullptr)
        return false;
    auto iter{std::find(attributes_.begin(), attributes_.end(), attr)};
    if (iter == attributes_.end())
        return false;
    attributes_.erase(iter);
    return true;
}

bool UMLClass::addParentClass(UMLClassifier *parentClass) {
    if (parentClass == nullptr)
        return false;
    auto iter{std::find(parentClasses_.begin(), parentClasses_.end(), parentClass)};
    if (iter != parentClasses_.end())
        return false;
    parentClasses_.insert(parentClass);
    return true;
}

bool UMLClass::removeParentClass(UMLClassifier *parentClass) {
    if (parentClass == nullptr)
        return false;
    auto iter{std::find(parentClasses_.begin(), parentClasses_.end(), parentClass)};
    if (iter == parentClasses_.end())
        return false;
    parentClasses_.erase(iter);
    return true;
}

UMLClassifier *UMLClass::removeParentClass(const std::string &parentClassName) {
    auto iter{std::find_if(parentClasses_.begin(), parentClasses_.end(),
                           [parentClassName](UMLClassifier *attr) { return attr->name() == parentClassName; })};
    UMLClassifier *found{nullptr};
    if (iter != parentClasses_.end()) {
        found = *iter;
        parentClasses_.erase(iter);
    }
    return found;
}

UMLClass::~UMLClass() {
    for (auto attr : attributes_) {
        delete attr;
    }
    for (auto relation : relations_) {
        // clean relations
    }
    attributes_.clear();
}