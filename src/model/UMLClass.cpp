/**
 * @brief Definition of UMLClass methods.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLClassifier.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */
#include "UMLClass.h"
#include "UMLOperation.h"
#include "UMLRelation.h"
#include <stdexcept>
#include <algorithm>
#include <QJsonArray>

UMLClass::UMLClass(const std::string& name, const std::vector<UMLAttribute *>& attributes) : UMLClassifier{name} {
    for (auto attribute : attributes) {
        addAttribute(attribute);
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

void UMLClass::addAttributes(const std::vector<UMLAttribute *> &attributes, bool cleanUnsuccessful) {
    for (auto attr : attributes) {
        if (attr == nullptr)
            continue;
        if (!addAttribute(attr) && cleanUnsuccessful) {
            delete attr;
        }
    }
}

UMLAttribute *UMLClass::getAttribute(const std::string &name) {
    // search for attribute with a given name
    auto iter{ std::find_if(attributes_.begin(), attributes_.end(),
                            [name](UMLAttribute *a) { return a->name() == name; })};
    if (iter != attributes_.end())
        return *iter;
    return nullptr;
}

std::unordered_set<UMLOperation *> UMLClass::getOperations() const {
    std::unordered_set<UMLOperation *> operations;
    for (auto a : attributes_) {
        if (dynamic_cast<UMLOperation *>(a) != nullptr)
            operations.insert(dynamic_cast<UMLOperation *>(a));
    }
    // get operations from base classes
    for (auto cls : parentClasses_) {
        for (auto a : cls->getOperations()) {
            operations.insert(a);
        }
    }
    return operations;
}

UMLAttribute *UMLClass::getAttributeAtPosition(int pos) {
    if (pos < 0 || pos >= attributes_.size())
        return nullptr;
    auto iter{attributes_.begin()};
    std::advance(iter, pos);
    return *iter;
}

long UMLClass::getAttributesPosition(UMLAttribute *attr) {
    if (attr == nullptr)
        return -1;
    auto iter{std::find(attributes_.begin(), attributes_.end(), attr)};
    if (iter == attributes_.end())
        return -1;
    return std::distance(attributes_.begin(), iter);
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
    attributes_.erase(iter);
    return *iter;
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

UMLRelation *UMLClass::addRelation(UMLClassifier *dst) {
    if (inRelationWith(dst))
        return nullptr;
    return new UMLRelation(this, dst);
}

bool UMLClass::removeRelation(UMLRelation *relation) {
    auto iter{std::find(relations_.begin(), relations_.end(), relation)};
    if (iter == relations_.end())
        return false;
    relations_.erase(iter);
    relation->removeRelationDependency(this);
    delete relation;
    return true;
}

bool UMLClass::removeRelation(UMLClassifier *dstClass) {
    if (!inRelationWith(dstClass))
        return false;
    auto iter{std::find_if(relations_.begin(), relations_.end(),
                           [&,dstClass](UMLRelation *r) { return r->compareClassesInRelation(this, dstClass); })};
    if (iter == relations_.end())
        return false;
    (*iter)->removeRelationDependency(this);
    auto tmpRel{*iter};
    relations_.erase(iter);
    delete tmpRel;
    return true;
}

UMLClass::~UMLClass() {
    for (auto attr : attributes_) {
        delete attr;
    }
}

void UMLClass::createJsonObject(QJsonObject &object) {
    UMLClassifier::createJsonObject(object);
    object["_class"] = "UMLClass";
    QJsonArray lofAttributes;
    for (auto attr : attributes_) {
        QJsonObject obj;
        attr->createJsonObject(obj);
        lofAttributes.push_back(obj);
    }
    object.insert("attributes", lofAttributes);
}
