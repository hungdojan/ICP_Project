//
// Created by rebulien on 4/21/22.
//

#ifndef DIAGRAMEDITOR_UMLRELATION_H
#define DIAGRAMEDITOR_UMLRELATION_H
#include "UMLClass.h"


class UMLRelation {
    UMLClass *src_;
    UMLClass *dst_;
    // TODO: relation type
    std::string srcMsg_;
    std::string dstMsg_;
public:
    UMLRelation() =delete;
    // Class constructor
    UMLRelation(UMLClass *src, UMLClass *dst);
    // TODO: constructor with relation type
    const UMLClass *src() const;
    const UMLClass *dst() const;
    std::string &srcMsg();
    std::string &dstMsg();
    // Adds this relation to bot source and destination classes
    bool setRelationDependency();
    // Swap source and destination classes
    void swapDirection();
    // Removes this relation from both source and destination classes
    void removeRelationDependency();
    // Compare if this relation is between two given classes
    bool compareClassesInRelation(const UMLClass *cls1, const UMLClass cls2);

    bool &operator==(const UMLRelation& relation);
};


#endif //DIAGRAMEDITOR_UMLRELATION_H
