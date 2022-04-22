//
// Created by rebulien on 4/21/22.
//

#ifndef DIAGRAMEDITOR_UMLRELATION_H
#define DIAGRAMEDITOR_UMLRELATION_H
#include "UMLClassifier.h"

class UMLRelation {
public:
    enum UMLRelationType {
        ASSOCIATION,
        DIR_ASSOCIATION,
        REF_ASSOCIATION,
        MULTIPLICITY,
        AGGREGATION,
        COMPOSITION,
        INHERITANCE,
        REALIZATION,
        UNDEFINED
    };
private:
    UMLClassifier *src_;
    UMLClassifier *dst_;
    UMLRelationType relationType_;
    std::string relationMsg_;
    std::string srcMsg_;
    std::string dstMsg_;
public:
    UMLRelation() =delete;
    // Class constructor
    explicit UMLRelation(UMLClassifier *src, UMLClassifier *dst);
    explicit UMLRelation(UMLClassifier *src, UMLClassifier *dst, UMLRelationType relationType);

    const UMLClassifier *src() const;
    const UMLClassifier *dst() const;
    void setRelationType(const UMLRelationType &relationType);
    const UMLRelationType &relationType() const;
    std::string &relationMsg();
    const std::string &relationMsg() const;
    std::string &srcMsg();
    const std::string &srcMsg() const;
    std::string &dstMsg();
    const std::string &dstMsg() const;
    // Adds this relation to bot source and destination classes
    bool setRelationDependency();
    // Swap source and destination classes
    void swapDirection();
    // Removes this relation from both source and destination classes
    void removeRelationDependency();
    // Compare if this relation is between two given classes
    bool compareClassesInRelation(const UMLClassifier *cls1, const UMLClassifier *cls2);

    // operator==(UMLRelation &relation, UMLRelation &relation2);
};


#endif //DIAGRAMEDITOR_UMLRELATION_H
