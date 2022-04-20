//
// Created by rebulien on 4/2/22.
//

#include <algorithm>
#include <list>
#include <utility>
#include <json/json.h>
#include "ClassDiagram.h"

void ClassDiagram::setName(std::string newName) {
    Element::setName(std::move(newName));
}

const std::vector<UMLClassifier *>& ClassDiagram::classElements() const {
    return classElements_;
}

const std::vector<SequenceDiagram *>& ClassDiagram::sequenceDiagrams() const {
    return sequenceDiagrams_;
}

void ClassDiagram::initClassDiagramFromFile(ClassDiagram &classDiagram, std::string path) {
    // TODO: name will be passed from the file content
}
void ClassDiagram::saveClassDiagramToFile(std::string path) {
    // TODO:
}

UMLClassifier *ClassDiagram::createClassifier(std::string name, bool isClass) {
    if (isClass)
        return new UMLClass(name);
    return new UMLClassifier(name);
}

bool ClassDiagram::addClassifier(UMLClassifier *classifier) {
    auto iter { std::find_if(classElements_.begin(), classElements_.end(),
                             [&classifier] (UMLClassifier *item) { return item->name() == classifier->name(); })};

    if (iter == classElements_.end()) {
        classElements_.push_back(classifier);
        return true;
    }
    return false;
}

UMLClassifier *ClassDiagram::addOrReplaceClassifier(UMLClassifier *classifier) {
    auto iter{ std::find_if(classElements_.begin(), classElements_.end(),
                            [classifier](UMLClassifier *c) { return c->name() == classifier->name(); })};

    UMLClassifier *found = nullptr;
    if (iter != classElements_.end()) {
        found = *iter;
        classElements_.erase(iter);
    }
    classElements_.push_back(classifier);
    return found;
}

void ClassDiagram::addClassifiers(std::vector<UMLClassifier *> classifiers) {
    for (auto item : classifiers) {
        addClassifier(item);
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

bool ClassDiagram::changeClassifierName(std::string oldName, std::string newName) {
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

UMLClassifier *ClassDiagram::getClassifier(std::string name) {
    auto iter{std::find_if(classElements_.begin(), classElements_.end(),
                           [name](UMLClassifier *c) { return name == c->name(); })};
    if (iter != classElements_.end())
        return *iter;
    return nullptr;
}

UMLClass *ClassDiagram::getClass(std::string name) {
    auto iter{std::find_if(classElements_.begin(), classElements_.end(),
                           [name](UMLClassifier *c) { return c->name() == name && dynamic_cast<UMLClass *>(c) != nullptr; })};

    if (iter != classElements_.end())
        return dynamic_cast<UMLClass *>(*iter);
    return nullptr;
}

bool ClassDiagram::isInClassDiagram(const UMLClassifier *classifier) {
    return std::find(classElements_.begin(), classElements_.end(), classifier) != classElements_.end();
}

// TODO: change return type
UMLClassifier *ClassDiagram::removeClassElement(std::string name) {
    auto iter{std::find_if(classElements_.begin(), classElements_.end(),
                           [name](UMLClassifier *c) { return c->name() == name; })};
    if (iter == classElements_.end())
        return nullptr;
    UMLClassifier *classifier{*iter};
    classElements_.erase(iter);
    return classifier;
}

bool ClassDiagram::removeClassElement(UMLClassifier *classElement) {
    auto iter{std::find(classElements_.begin(), classElements_.end(), classElement)};
    if (iter == classElements_.end())
        return false;
    classElements_.erase(iter);
    return true;
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