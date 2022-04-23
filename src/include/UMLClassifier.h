//
// Created by rebulien on 4/2/22.
//

#ifndef DIAGRAMEDITOR_UMLCLASSIFIER_H
#define DIAGRAMEDITOR_UMLCLASSIFIER_H

#include "Element.h"
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>

class UMLAttribute;
class UMLRelation;

class UMLClassifier : public Element {
protected:
    // used for factory function
    friend class ClassDiagram;
    friend class UMLRelation;
    bool isUserDefined_;
    bool isAbstract_{false};
    std::unordered_set<UMLClassifier *> parentClasses_;
    std::unordered_set<UMLRelation *> relations_;
    explicit UMLClassifier(std::string name, bool isUserDefined) : Element{std::move(name)}, isUserDefined_{isUserDefined} { }

    virtual bool addParentClass(UMLClassifier *parentClass);
    virtual bool removeParentClass(UMLClassifier *parentClass);
    virtual UMLClassifier *removeParentClass(const std::string &name);
public:
    UMLClassifier() =delete;
    explicit UMLClassifier(std::string name) : Element{std::move(name)}, isUserDefined_{false} { };
    static UMLAttribute *createAttribute(bool isOperation, const std::string &name, UMLClassifier *type, const std::vector<UMLAttribute> &params);
    const bool &isUserDefined() const;
    const bool &isAbstract() const;
    bool &isAbstract();
    const std::unordered_set<UMLClassifier *> &parentClasses() const;
    const std::unordered_set<UMLRelation *> &relations() const;

    void setName(std::string name);
    virtual bool containsParentClass(UMLClassifier *parentClass) const;

    virtual bool addRelation(UMLClassifier *dst);
    virtual bool inRelationWith(UMLClassifier *dst) const;
    virtual UMLRelation *getRelationWith(UMLClassifier *dst);
    virtual bool removeRelation(UMLRelation *relation);
    virtual bool removeRelation(UMLClassifier *dstClass);

    ~UMLClassifier() override;
};


#endif //DIAGRAMEDITOR_UMLCLASSIFIER_H
