/**
 * @brief Declaration of ClassDiagram class.
 * ClassDiagram class represents abstract structure that holds class diagram data.
 * There are elements like classes, interfaces and sequence diagram stored in the class.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file ClassDiagram.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_CLASSDIAGRAM_H
#define DIAGRAMEDITOR_CLASSDIAGRAM_H
#include <vector>
#include "Element.h"
#include "UMLClassifier.h"
#include "UMLClass.h"
#include "SequenceDiagram.h"
#include <QJsonObject>

/**
 * @brief Representation of class diagram.
 */
class ClassDiagram : public Element {
protected:
    /** Collection of class elements (classifiers) */
    std::vector<UMLClassifier *> classElements_;
    /** Collection of sequence diagrams */
    std::vector<SequenceDiagram *> sequenceDiagrams_;
public:
    /**
     * @brief Types of class element.
     */
    enum ClassElementType {
        CLASSIFIER, CLASS, INTERFACE
    };

    ClassDiagram() =delete;

    /**
     * Creates new ClassDiagram object.
     * @brief Class constructor
     * @param name Class diagram's name.
     */
    explicit ClassDiagram(const std::string& name) : Element{name}, classElements_{}, sequenceDiagrams_{} { };

    /**
     * @brief Updates ClassDiagram's name.
     * @param newName New name.
     */
    void setName(const std::string& newName) override;

    /**
     * @brief Returns read-only collections of class elements.
     * @return Collection of class elements.
     */
    const std::vector<UMLClassifier *>& classElements() const;

    /**
     * @brief Returns read-only collections of sequence diagrams.
     * @return Collection of sequence diagrams.
     */
    const std::vector<SequenceDiagram *>& sequenceDiagrams() const;

    /**
     * Factory method that creates new instance of class elements. Attributes has to be added afterwards.
     * New instance is NOT stored anywhere yet. If user don't insert it into class diagram user has to
     * dealloc resources themselves.
     * @brief Creates new instance of class element.
     * @param name Class element's name.
     * @param classElementType Class element type.
     * @return New instance of class element.
     */
    static UMLClassifier *createClassifier(const std::string& name, ClassElementType classElementType);

    /**
     * Adds new class element into the class diagram.
     * When another class element with identical name already exists in the class diagram,
     * insertion is ABORTED and OLD instance is KEPT in class diagram. Name check is case-sensitive.
     * @brief Inserts new classifier into class diagram.
     * @param classifier instance of classifier.
     * @return Whether insertion ended successfully.
     */
    bool addClassifier(UMLClassifier *classifier);

    /**
     * Adds or replace class element in the class diagram.
     * When another class element with identical name already exists in the class diagram,
     * NEW one is inserted and OLD one is REMOVED and RETURNED for user to handle cleaning up resources.
     * @brief Insert or replace classifier in class diagram. Name check is case-sensitive.
     * @param classifier Instance of classifier.
     * @return OLD instance found with same name found in diagram, nullptr if no element with identical name was found.
     */
    UMLClassifier *addOrReplaceClassifier(UMLClassifier *classifier);

    // Adds multiple classifiers into the class diagram.
    /**
     * Adds collection of class elements into the class diagram. Use _cleanUnsuccessful_ flag
     * to define, whether to dealloc objects that weren't able to insert.
     * @brief Insert collection of classifiers into diagram.
     * @param classifiers Collection of classifiers.
     * @param cleanUnsuccessful Whether to dealloc objects that couldn't be inserted.
     */
    void addClassifiers(const std::vector<UMLClassifier *>& classifiers, bool cleanUnsuccessful);

    /**
     * Change class element's name. New name MUST NOT collide with already existing elements,
     * meaning new name can be only set when no class element has identical name. Name check is case-sensitive.
     * @brief Updates class element's name.
     * @param classifier Instance of classifier.
     * @param name New name to set.
     * @return Whether function was able to change classifier's name.
     */
    bool changeClassifierName(UMLClassifier *classifier, const std::string& name);

    /**
     * Change class element's name. New name MUST NOT collide with already existing elements,
     * meaning new name can be only set when no class element has identical name. Name check is case-sensitive.
     * @brief Updates class element's name.
     * @param classifier Instance of classifier.
     * @param name New name to set.
     * @return Whether function was able to change classifier's name.
     */
    bool changeClassifierName(const std::string& oldName, const std::string& newName);

    /**
     * @brief Search for class element in the class diagram by name.
     * @param name Class element's name.
     * @return Pointer to found class element, nullptr otherwise.
     */
    UMLClassifier *getClassifier(const std::string &name) const ;

    /**
     * @brief Checks if given classifier is in the class diagram.
     * @param classifier Instance of classifier.
     * @return Whether function was able to find instance of element in the diagram.
     */
    bool isInClassDiagram(const UMLClassifier *classifier) const ;

    // Removes classifier from the class diagram
    /**
     * Removes class element from the class diagram by name.
     * When class element is found in the diagram, it is removed from the diagram and returned to user
     * to handle the resource deletion.
     * @brief Removes classifier from the class diagram.
     * @param name Class element's name.
     * @return Instance of removed element if found, nullptr when nothing was removed.
     */
    UMLClassifier *removeClassElement(const std::string &name);

    // Removes classifier from the class diagram
    /**
     * Removes class element from the class diagram if found.
     * User has to handle resource deletion.
     * @brief Removes classifier from the class diagram.
     * @param classElement Instance of class element.
     * @return Whether removal was successful. When function returns true, user can cleanup resources.
     */
    bool removeClassElement(UMLClassifier *classifier);

    /**
     * @brief Creates JSON representation of element's content.
     */
    void createObject(QJsonObject &object) override;

    // sequence diagram methods
    // TODO: add, search, remove
    // Class destructor
    ~ClassDiagram() override;
};

#endif //DIAGRAMEDITOR_CLASSDIAGRAM_H