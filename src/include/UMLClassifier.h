/**
 * @brief Declaration of UMLClassifier class.
 * UMLClassifier represents abstract structure of data type/structure. UMLClass and UMLInterface derive from this class.
 * User will mostly use UMLClass and UMLInterface to display elements from the class diagram but for primitive data in
 * attributes and functions/methods user can create "dummy" UMLClassifier. This class is semi-abstract meaning some methods
 * in this class are only meant to exist for polymorphism.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLClassifier.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_UMLCLASSIFIER_H
#define DIAGRAMEDITOR_UMLCLASSIFIER_H

#include "Element.h"
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>
#include <QJsonObject>
#include "ISubject.h"

// Declaration of classes that are implemented in different source file
class UMLAttribute;
class UMLRelation;
class UMLOperation;

/**
 * @brief Representation of any kind of data type or element in class diagram.
 */
class UMLClassifier : public Element, public ISubject {
protected:
    // set visibility for other classes
    friend class ClassDiagram;
    friend class UMLRelation;
    /**
     * Separates user-defined classes from
     * built-in or primitive data types (for example used in attributes and methods
     */
    bool isUserDefined_;
    /** Abstract classifiers cannot create objects in sequence diagram */
    bool isAbstract_{false};
    /** Collection of direct base classes of this classifier */
    std::unordered_set<UMLClassifier *> parentClasses_;
    /** Collection of relations created between this and other (user-defined) classifier */
    std::unordered_set<UMLRelation *> relations_;
    /** X and Y coordinates of their gElement instances. */
    double x_, y_;

    /**
     * @brief Class constructor.
     * @param name Classifier's name.
     * @param isUserDefined Whether classifier is user-defined or not.
     */
    explicit UMLClassifier(std::string name, bool isUserDefined) : Element{std::move(name)}, isUserDefined_{isUserDefined}, x_{0}, y_{0} { }

    /**
     * @brief Adds direct base class to the collection.
     * This function adds new classifier into the collection of parent classes. That allow class/instance of class to
     * retrieve list of operations that object can use.
     * If parent class is already in the collection, function is halted.
     * @param parentClass New base class.
     * @return Whether insertion ended successfully.
     */
    virtual bool addParentClass(UMLClassifier *parentClass);

    /**
     * @brief Removes classifier from the direct base classes.
     * Function is ignored when trying to remove classifier that isn't in the collection of base classifiers.
     * @param parentClass Base class to remove.
     * @return Whether removal ended successfully.
     */
    virtual bool removeParentClass(UMLClassifier *parentClass);

    /**
     * @brief Removes classifier from the direct base classes.
     * Function is ignored when trying to remove classifier that isn't in the collection of base classifiers.
     * Class is search by name. If this classifier contains more classes with similar name (usually happens when user is
     * inserting classifier that is not within class diagram), function removes instance of first encounter from the collection.
     * @param parentClass Base class to remove.
     * @return Whether removal ended successfully.
     */
    virtual UMLClassifier *removeParentClass(const std::string &name);
public:
    UMLClassifier() =delete;
    /**
     * @brief Class constructor.
     * @param name Classifier's name.
     */
    explicit UMLClassifier(std::string name) : Element{std::move(name)}, isUserDefined_{false}, x_{0}, y_{0} { };

    /**
     * @brief Class constructor.
     * @param name Classifier's name.
     * @param x Classifier's x-coordinate.
     * @param y Classifier's y-coordinate.
     */
    explicit UMLClassifier(std::string name, double x, double y) : Element{std::move(name)}, isUserDefined_{false}, x_{x}, y_{y} { };

    /**
     * @brief Factory method to create attribute and operation/method.
     * This function creates instance of attribute/operation by given parameters. To create attribute or operation with
     * empty list of parameters, user can parse empty vector {} to _params_ parameter. When initialization failed
     * resource in vector are NOT CLEARED. User has to clear resources by themselves. When user tries to create attribute
     * with parameters function ignores parameters.
     * @param isOperation Whether to create operation/method or attribute.
     * @param name Attribute's name.
     * @param type Attribute's (return) type.
     * @param params Vector of parameters
     * @return Created instance of attribute/operation.
     */
    static UMLAttribute *createAttribute(bool isOperation, const std::string &name, UMLClassifier *type, const std::vector<UMLAttribute *> &params);

    /**
     * @brief Returns value of isUserDefined_.
     * @return Whether classifier is user-defined.
     */
    const bool &isUserDefined() const;

    /**
     * @brief Returns value of isAbstract_.
     * @return Whether classifier is abstract.
     */
    const bool &isAbstract() const;

    /**
     * @brief Returns reference of isAbstract_ property.
     * @return Reference of isAbstract_ property.
     */
    bool &isAbstract();

    /**
     * @brief Returns read-only collection of direct base classes.
     * @return Read-only collection of direct base classes.
     */
    const std::unordered_set<UMLClassifier *> &parentClasses() const;

    /**
     * @brief Returns read-only collection of relations.
     * @return Read-only collection of relations.
     */
    const std::unordered_set<UMLRelation *> &relations() const;

    /**
     * @brief Returns read-only value of element's x-coordinate.
     * @return Read-only value of x-coordinate.
     */
    const double &x() const;

    /**
     * @brief Returns read-only value of element's y-coordinate.
     * @return Read-only value of y-coordinate.
     */
    const double &y() const;

    /**
     * @brief Returns reference to element's x-coordinate.
     * @return Reference to x-coordinate.
     */
    double &x();

    /**
     * @brief Returns reference to element's y-coordinate.
     * @return Reference to y-coordinate.
     */
    double &y();

    /**
     * @brief Updates classifier's name.
     * This function should not be use directly in class diagram due to missing duplicity of name check.
     * For that use ClassDiagram::changeClassifierName(). When classifier is not defined within diagram
     * then is safe to use.
     * @param name New classifier's name.
     */
    void setName(std::string name);

    /**
     * @brief Returns collection of callable operations of this class.
     * @return Collection of callable operations.
     */
    virtual std::unordered_set<UMLOperation *> getOperations() const;

    /**
     * @brief Checks if this class derive from given classifier.
     * @param parentClass Base classifier.
     * @return Whether this classifier derive from _parentClass_.
     */
    virtual bool containsParentClass(UMLClassifier *parentClass) const;

    /**
     * @brief Creates new relation between this classifier and given classifier.
     * When relation between this and given classifier already exists, operation is halted.
     * New relation is also added to destination classifier.
     * @param dst Destination classifier.
     * @throw invalid_argument Cannot be used with UMLClassifier.
     * @return Instance of created relation, nullptr otherwise.
     */
    virtual UMLRelation *addRelation(UMLClassifier *dst);

    /**
     * @brief Checks if there is a relation formed between this and given classifier.
     * @param dst Destination classifier.
     * @return Whether there is a relation between this and given classifier.
     */
    virtual bool inRelationWith(UMLClassifier *dst) const;

    /**
     * @brief Returns instance of relation.
     * When no relation is found, function returns nullptr.
     * @param dst Destination classifier.
     * @return Instance of relation between this and given classifier, nullptr when no instance found.
     */
    virtual UMLRelation *getRelationWith(UMLClassifier *dst);

    /**
     * @brief Removes relation from this classifier.
     * The instance of relation is also removed from the destination other classifier.
     * Resources of relation parameter are CLEARED WITHIN THIS FUNCTION. Content of parameter
     * _relation_ is undefined if operation returns true.
     * @param relation Instance of relation.
     * @throw invalid_argument Cannot be used with UMLClassifier.
     * @return Whether relation was found and removed.
     */
    virtual bool removeRelation(UMLRelation *relation);

    /**
     * @brief Removes relation between this and given classifier.
     * The instance of relation is also removed from the destination other classifier.
     * Resources of relation parameter are CLEARED WITHIN THIS FUNCTION.
     * @param dstClass Destination class
     * @throw invalid_argument Cannot be used with UMLClassifier.
     * @return Whether relation was found and removed.
     */
    virtual bool removeRelation(UMLClassifier *dstClass);

    /**
     * @brief Clear all relations in the classifier.
     */
    virtual void clearRelations();

    /**
     * @brief Adds observer into the collection of observers.
     * @param observer Instance of observer.
     */
    void attach(IObserver *observer) override;

    /**
     * @brief Removes observer from the collection of observers.
     * @param observer Instance of observer.
     */
    void detach(IObserver *observer) override;

    /**
     * @brief Sends message to all subscribers/observers.
     * @param msg Message content.
     */
    void notify(const std::string &msg) override;

    /**
     * @brief Returns number of observers of this classifier.
     * @return Number of observers of this classifier.
     */
    unsigned long observerCount() const;

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createJsonObject(QJsonObject &object) override;

    /**
     * @brief Class destructor.
     */
    ~UMLClassifier() override;
};


#endif //DIAGRAMEDITOR_UMLCLASSIFIER_H
