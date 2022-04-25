/**
 * @brief Definition of ClassDiagram class.
 * This file contains implementations of all functions of ClassDiagram class.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22
 *
 * @file ClassDiagram.cpp
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include <algorithm>
#include <list>
#include "ClassDiagram.h"
#include "UMLInterface.h"
#include "UMLRelation.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <fstream>

void ClassDiagram::setName(const std::string& newName) {
    Element::setName(newName);
}

const std::vector<UMLClassifier *>& ClassDiagram::classElements() const {
    return classElements_;
}

const std::vector<SequenceDiagram *>& ClassDiagram::sequenceDiagrams() const {
    return sequenceDiagrams_;
}

UMLClassifier *ClassDiagram::createClassifier(const std::string& name, ClassElementType classElementType) {
    switch(classElementType) {
        case CLASSIFIER:
            return new UMLClassifier(name, true);
        case CLASS:
            return new UMLClass(name);
        case INTERFACE:
            return new UMLInterface(name);
        default:
            return nullptr;
    }
}

bool ClassDiagram::addClassifier(UMLClassifier *classifier) {
    // search for class element with identical name
    // ignore insertion when diagram contain class element with identical name
    auto iter { std::find_if(classElements_.begin(), classElements_.end(),
                             [&classifier] (UMLClassifier *item) { return item->name() == classifier->name(); })};

    if (iter == classElements_.end()) {
        classElements_.push_back(classifier);
        return true;
    }
    return false;
}

UMLClassifier *ClassDiagram::addOrReplaceClassifier(UMLClassifier *classifier) {
    // search for class element with given name
    // remove it and replace it with _classifier_ when instance found
    auto iter{ std::find_if(classElements_.begin(), classElements_.end(),
                            [classifier](UMLClassifier *c) { return c->name() == classifier->name(); })};

    UMLClassifier *found = nullptr;
    if (iter != classElements_.end()) {
        found = *iter;
        classElements_.erase(iter);
    }
    classElements_.push_back(classifier);
    // return result for user to handle resources
    return found;
}

void ClassDiagram::addClassifiers(const std::vector<UMLClassifier *>& classifiers, bool cleanUnsuccessful) {
    for (auto item : classifiers) {
        // deletes classifiers when classifier with same name already in the diagram
        if (item == nullptr)
            continue;
        if (!addClassifier(item) && cleanUnsuccessful) {
            delete item;
        }
    }
}

bool ClassDiagram::changeClassifierName(UMLClassifier *classifier, const std::string& name) {
    // check parameters
    // list ClassDiagram::classElements_ must contain instance of _classifier_ parameter
    // and must not contain instance of classifier that has identical name as _name_
    auto existenceOfItem{std::find(classElements_.begin(), classElements_.end(), classifier)};
    auto itemIdenticalName{std::find_if(classElements_.begin(), classElements_.end(),
                                        [name](UMLClassifier *c) { return c->name() == name; })};

    if (existenceOfItem != classElements_.end() && itemIdenticalName == classElements_.end()) {
        (*existenceOfItem)->setName(name);
        return true;
    }
    return false;
}

bool ClassDiagram::changeClassifierName(const std::string& oldName, const std::string& newName) {
    // check parameters
    // list ClassDiagram::classElements_ must contain classifier named _oldName_
    // and must not contain instance of classifier that has identical name as _newName_
    auto existenceOfItem{std::find_if(classElements_.begin(), classElements_.end(),
                                      [oldName](UMLClassifier *c) { return c->name() == oldName; })};
    auto itemIdenticalName{std::find_if(classElements_.begin(), classElements_.end(),
                                        [newName](UMLClassifier *c) { return c->name() == newName; })};
    if (existenceOfItem != classElements_.end() && itemIdenticalName == classElements_.end()) {
        (*existenceOfItem)->setName(newName);
        return true;
    }
    return false;
}

UMLClassifier *ClassDiagram::getClassifier(const std::string &name) const {
    // search for class element within diagram
    // return result
    auto iter{std::find_if(classElements_.begin(), classElements_.end(),
                           [name](UMLClassifier *c) { return name == c->name(); })};
    if (iter != classElements_.end())
        return *iter;
    return nullptr;
}

bool ClassDiagram::isInClassDiagram(const UMLClassifier *classifier) const {
    return std::find(classElements_.begin(), classElements_.end(), classifier) != classElements_.end();
}

UMLClassifier *ClassDiagram::removeClassElement(const std::string &name) {
    // if search for class element ends up successfully
    // remove it and return in to user to handle resources
    auto iter{std::find_if(classElements_.begin(), classElements_.end(),
                           [name](UMLClassifier *c) { return c->name() == name; })};
    if (iter == classElements_.end())
        return nullptr;
    classElements_.erase(iter);
    return *iter;
}

bool ClassDiagram::removeClassElement(UMLClassifier *classifier) {
    auto iter{std::find(classElements_.begin(), classElements_.end(), classifier)};
    if (iter == classElements_.end())
        return false;
    classElements_.erase(iter);
    return true;
}

void ClassDiagram::createJsonObject(QJsonObject &object) {
    object.insert("_class", "ClassDiagram");
    object.insert("name", QString::fromStdString(name_));
    QJsonArray qClassElements, qSequenceDiagram, qRelations;
    std::unordered_set<UMLRelation *> setOfRelations;
    for (auto classifier : classElements_) {
        QJsonObject obj;
        classifier->createJsonObject(obj);
        qClassElements.push_back(obj);
        for (auto r : classifier->relations()) {
            setOfRelations.insert(r);
        }
    }
    object.insert("classElements", qClassElements);

    // TODO: sequence diagram
    object.insert("sequenceDiagrams", qSequenceDiagram);

    for (auto r : setOfRelations) {
        QJsonObject obj;
        r->createObject(obj);
        qRelations.push_back(obj);
    }
    object.insert("relations", qRelations);
}

ClassDiagram::~ClassDiagram() {
    for (auto i : classElements_) {
        delete i;
    }
    for (auto i : sequenceDiagrams_) {
        delete i;
    }
    classElements_.clear();
    sequenceDiagrams_.clear();
}
