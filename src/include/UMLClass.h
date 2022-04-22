//
// Created by rebulien on 4/5/22.
//

#ifndef DIAGRAMEDITOR_UMLCLASS_H
#define DIAGRAMEDITOR_UMLCLASS_H
#include "UMLClassifier.h"
#include "UMLAttribute.h"
#include <vector>
#include <list>
#include <unordered_set>

class UMLRelation;

class UMLClass : public UMLClassifier {
    std::unordered_set<UMLClassifier *> parentClasses_;
    std::vector<UMLAttribute *> attributes_;
    std::unordered_set<UMLRelation *> relations_;
    std::string stereotype_;
public:
    UMLClass() =delete;
    // Class constructor
    explicit UMLClass(const std::string &name) : UMLClassifier{name} { };
    // Class constructor
    explicit UMLClass(const std::string& name, const std::vector<UMLAttribute *>&attributes);

    // public getter and setter
    const std::string &stereotype() const;
    std::string &stereotype();
    // public getter, private setter
    const std::vector<UMLAttribute *> &attributes() const;
    // public getter, private setter
    const std::unordered_set<UMLClassifier *> &parentClasses() const;

    // TODO: add getter and setter for relations
    // Adds new attribute to hte class
    bool addAttribute(UMLAttribute *attr);
    // Adds new attribute to the class
    UMLAttribute *addOrReplaceAttribute(UMLAttribute *attr);
    // Returns instance of UMLAttribute in the class by a given name
    UMLAttribute *getAttribute(const std::string &name);
    //Returns collection of all callable operations_
    const std::list<UMLAttribute *> getOperations() const;
    // Returns attribute from a given position
    UMLAttribute *getAttributeAtPosition(int pos);
    // Get attributes position in the class
    int getAttributesPosition(UMLAttribute *attr);
    // Move attribute of a given name to the given position
    bool moveAttributeToPosition(const std::string &name, int pos);
    // Updates attribute's name
    bool updateAttributeName(UMLAttribute *attribute, const std::string &newName);
    // Updates attribute's name
    bool updateAttributeName(std::string oldName, std::string newName);
    // Removes attribute by a given name from the class
    UMLAttribute *removeAttribute(const std::string &name);
    // Removes attribute from the class
    bool removeAttribute(UMLAttribute *attr);
    // Adds new parent class
    bool addParentClass(UMLClassifier *parentClass);
    // Removes class from the set of parent classes
    bool removeParentClass(UMLClassifier *parentClass);
    // Removes class from the set of parent classes
    UMLClassifier *removeParentClass(const std::string &parentClassName);

    // TODO: function for relations
    // TODO: destructor
    ~UMLClass();
};


#endif //DIAGRAMEDITOR_UMLCLASS_H
