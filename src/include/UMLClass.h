/**
 * @brief Declaration of UMLClass class.
 * UMLClass represents abstract structure of user-defined class in the class diagram. It contains attributes and
 * operations that objects from this class can use in sequence diagram. Class can be set as abstract meaning
 * sequence diagrams cannot create object from this class. Class can derive from other classes or form other types
 * of relations between them.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLClassifier.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_UMLCLASS_H
#define DIAGRAMEDITOR_UMLCLASS_H
#include "UMLClassifier.h"
#include "UMLAttribute.h"
#include <vector>
#include <list>
#include <unordered_set>

class UMLClass : public UMLClassifier {
protected:
    /** Collection of attributes and operations of the class */
    std::vector<UMLAttribute *> attributes_;
    /** Special stereotype to display in the diagram */
    std::string stereotype_;
public:
    UMLClass() =delete;

    /**
     * @brief Class constructor.
     * @param name Class's nmae.
     */
    explicit UMLClass(const std::string &name) : UMLClassifier{name, true} { };

    /**
     * @brief Class constructor.
     * @param name Class's name.
     * @param attributes Collection of attributes of the class.
     */
    explicit UMLClass(const std::string& name, const std::vector<UMLAttribute *>&attributes);

    /**
     * @brief Returns value of stereotype.
     * @return Read-only reference to stereotype property.
     */
    const std::string &stereotype() const;

    /**
     * @brief Returns value of stereotype.
     * @return Modifiable reference to stereotype property.
     */
    std::string &stereotype();

    /**
     * @brief Returns reference to class attributes and operations.
     * @return Read-only reference to collection of attributees and operations.
     */
    const std::vector<UMLAttribute *> &attributes() const;

    /**
     * @brief Adds attribute to class.
     * Adds attribute to the class. When class already contains attribute with identical name in it
     * (doesn't matter whether it is attribute ar operation), OLD instance will be PRESERVED in the class
     * and new instance will be ignored.
     * @param attr Instance of attribute.
     * @return Whether attribute was successfully added.
     */
    bool addAttribute(UMLAttribute *attr);

    /**
     * @brief Adds attribute to class.
     * Adds attribute to the class. When class already contains attribute with identical name in it
     * (doesn't matter whether it is attribute ar operation), old one will be REMOVED from the class and returned to user.
     * @param attr Instance of attribute.
     * @return Whether attribute was successfully added.
     */
    UMLAttribute *addOrReplaceAttribute(UMLAttribute *attr);

    /**
     * @brief Returns instance of UMLAttribute in the class by given name.
     * @param name Attribute's name.
     * @return Instance of found attribute, nullptr otherwise.
     */
    UMLAttribute *getAttribute(const std::string &name);

    /**
     * @brief Returns collection of callable operations of this class.
     * @return Collection of callable operations.
     */
    std::unordered_set<UMLAttribute *> getOperations() const override;

    // Returns attribute from a given position
    /**
     * @brief Returns attribute that is in given position.
     * Nullptr is returned when given position is out of range of the set.
     * @param pos Index of attribute to return.
     * @return Instance of found attribute, nullptr when not found.
     */
    UMLAttribute *getAttributeAtPosition(int pos);

    // Get attributes position in the class
    /**
     * @brief Returns attribute's position.
     * When attribute is not found in the class, -1 value is returned.
     * @param attr Instance of attribute.
     * @return Attribute's position in the class, -1 when not found.
     */
    long getAttributesPosition(UMLAttribute *attr);

    /**
     * @brief Updates attribute's name.
     * Updates attribute's name. When attribute is not found in the class or newName si already occupied,
     * function ignores operation.
     * @param attribute Instance of attribute.
     * @param newName New attribute's name.
     * @return Whether updating name ended successfully.
     */
    bool updateAttributeName(UMLAttribute *attribute, const std::string &newName);

    /**
     * @brief Updates attribute's name.
     * Updates attribute's name. When attribute wil oldName is not found or newName is already occupied,
     * function ignores operation.
     * @param oldName Name of attribute to update name.
     * @param newName New attribute's name.
     * @return Whether updating name ended successfully.
     */
    bool updateAttributeName(std::string oldName, std::string newName);

    /**
     * @brief Removes attribute from the class.
     * @param name Name of the attribute.
     * @return Whether removal ended successfully.
     */
    UMLAttribute *removeAttribute(const std::string &name);

    /**
     * @brief Removes attribute from the class.
     * @param attr Instance of attribute.
     * @return Whether removal ended successfully.
     */
    bool removeAttribute(UMLAttribute *attr);

    /**
     * @brief Creates new relation between this classifier and given classifier.
     * When relation between this and given classifier already exists, operation is halted.
     * New relation is also added to destination classifier.
     * @param dst Destination classifier.
     * @return Instance of created relation, nullptr otherwise.
     */
    UMLRelation *addRelation(UMLClassifier *dst) override;

    /**
     * @brief Removes relation from this classifier.
     * The instance of relation is also removed from the destination other classifier.
     * Resources of relation parameter are CLEARED WITHIN THIS FUNCTION. Content of parameter
     * _relation_ is undefined if operation returns true.
     * @param relation Instance of relation.
     * @return Whether relation was found and removed.
     */
    bool removeRelation(UMLRelation *relation)  override;

    /**
     * @brief Removes relation between this and given classifier.
     * The instance of relation is also removed from the destination other classifier.
     * Resources of relation parameter are CLEARED WITHIN THIS FUNCTION.
     * @param dstClass Destination class
     * @return Whether relation was found and removed.
     */
    bool removeRelation(UMLClassifier *dstClass) override;

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createObject(QJsonObject &object) override;

    /**
     * Class destructor
     */
    ~UMLClass() override;
};

#endif //DIAGRAMEDITOR_UMLCLASS_H
