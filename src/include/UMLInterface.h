/**
 * @brief Declaration of UMLInterface.
 * UMLClass represents abstract structure of user-defined interface in the class diagram.
 * It only contains collection of operations. User cannot create object from this class, but other class elements
 * can inherit data from this element.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLInterface.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_UMLINTERFACE_H
#define DIAGRAMEDITOR_UMLINTERFACE_H
#include <vector>
#include <unordered_set>
#include "UMLClassifier.h"
#include "UMLOperation.h"
#include "ISubject.h"

/**
 * @brief Representation of interface in class diagram.
 */
class UMLInterface : public UMLClassifier {
protected:
    /** Collection of operations */
    std::vector<UMLOperation *> operations_;
    using UMLClassifier::isAbstract;
public:
    UMLInterface() =delete;
    /**
     * @brief Class constructor.
     * @param name Interface name.
     */
    explicit UMLInterface(const std::string &name);

    /**
     * @brief Class constructor.
     * @param name Interface name.
     * @param operations Collection of operations.
     */
    explicit UMLInterface(const std::string &name, const std::vector<UMLOperation *> &operations);

    /**
     * @brief Adds operation into this interface.
     * If operation with same name already exists in this interface
     * OLD instance IS KEPT.
     * @param operation Instance of operation.
     * @return Whether insertion ended successfully.
     */
    bool addOperation(UMLOperation *operation);

    /**
     * @brief Inserts indefinite number of operations into this interface.
     * @param operations Collection of operations.
     */
    void addOperations(const std::vector<UMLOperation *> &operations);

    /**
     * @brief Extracts operation from the interface.
     * @param name Operation name.
     * @return Instance of operation if found, nullptr otherwise.
     */
    UMLOperation *getOperation(const std::string& name) const;

    /**
     * @brief Searches for operation in the interface.
     * @param operation Instance of operation.
     * @return Whether given operation is in the interface.
     */
    bool containsOperation(const UMLOperation *operation) const;

    /**
     * @brief Returns collection of operations in this interface.
     * @return Collection of operations.
     */
    const std::vector<UMLOperation *> &operations() const;

    /**
     * @brief Removes operation from the interface.
     * @param operation Instance of operation.
     * @return Whether removal ended successfully.
     */
    bool removeOperation(UMLOperation *operation);

    /**
     * @brief Returns collection of callable operations of this class.
     * @return Collection of callable operations.
     */
    std::unordered_set<UMLOperation *> getOperations() const override;

    /**
     * @brief Creates new relation with this interface.
     * If instance already exists method return nullptr.
     * @param dst Destination classifier.
     * @return Instance of relation when successfully added.
     */
    UMLRelation *addRelation(UMLClassifier *dst) override;

    /**
     * @brief Removes relation from this interface.
     * @param relation Instance of relation.
     * @return Whether removal ended successfully.
     */
    bool removeRelation(UMLRelation *relation) override;

    /**
     * @brief Removes relation with destination class element.
     * @param dstClass Destination element.
     * @return Whether removal ended successfully.
     */
    bool removeRelation(UMLClassifier *dstClass) override;

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createJsonObject(QJsonObject &object) override;

    /**
     * Class destructor.
     */
    ~UMLInterface() override;
};

#endif //DIAGRAMEDITOR_UMLINTERFACE_H