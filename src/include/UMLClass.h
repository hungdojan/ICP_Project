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

class UMLClass : public UMLClassifier {
    std::unordered_set<UMLClass *> parentClasses_;
    std::vector<UMLAttribute *> attributes_;
    std::string stereotype_;
    bool isAbstract_;
    // TODO: add relations
    // TODO: add parentClasses
public:
    UMLClass() =delete;
    // Class constructor
    explicit UMLClass(const std::string &name) : UMLClassifier{name}, isAbstract_{false} { };
    // Class constructor
    explicit UMLClass(const std::string& name, const std::vector<UMLAttribute *>&attributes);

    // public getter and setter
    bool &isAbstract();
    // public getter and setter
    std::string &stereotype();
    // public getter, private setter
    const std::vector<UMLAttribute *> &attributes() const;
    // public getter, private setter
    const std::unordered_set<UMLClass *> &parentClasses() const;

    // TODO: add getter and setter for relations
    // Factory function that creates new attribute
    static UMLAttribute *createAttribute(bool isOperation, const std::string& name, UMLClassifier *type, const std::vector<UMLAttribute> &parameters);
    // Adds new attribute to hte class
    bool addAttribute(UMLAttribute *attr);
    // Adds new attribute to the class
    UMLAttribute *addOrReplaceAttribute(UMLAttribute *attr);
    // Returns instance of UMLAttribute in the class by a given name
    UMLAttribute *getAttribute(const std::string &name);
    //Returns collection of all callable operations
    const std::list<UMLAttribute *>getOperations() const;
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
    bool addParentClass(UMLClass *parentClass);
    // Removes class from the set of parent classes
    bool removeParentClass(UMLClass *parentClass);
    // Removes class from the set of parent classes
    UMLClass *removeParentClass(const std::string &parentClassName);

    // TODO: function for relations
    ~UMLClass();
};


#endif //DIAGRAMEDITOR_UMLCLASS_H
