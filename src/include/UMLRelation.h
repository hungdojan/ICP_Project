/**
 * @brief Declaration of class UMLRelation.
 * UMLRelation represents abstract structure of relation between two class elements in the class diagram.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLRelation.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_UMLRELATION_H
#define DIAGRAMEDITOR_UMLRELATION_H
#include "UMLClassifier.h"

/**
 * @brief Representation of relation in class diagram.
 */
class UMLRelation {
public:
    /**
     * @brief Types of Relations.
     */
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
    /** Source class element. */
    UMLClassifier *src_;
    /** Destination class element. */
    UMLClassifier *dst_;
    /** Relation type. */
    UMLRelationType relationType_;
    /** Additional relation content. */
    std::string relationMsg_;
    /** Source message to display */
    std::string srcMsg_;
    /** Destination message to display */
    std::string dstMsg_;
public:
    UMLRelation() =delete;
    /**
     * @brief Class constructor.
     * Default value of relation type is ASSOCIATION.
     * @param src Source class element.
     * @param dst Destination class element.
     */
    explicit UMLRelation(UMLClassifier *src, UMLClassifier *dst);

    /**
     * @brief Class constructor.
     * @param src Source class element.
     * @param dst Destination class element.
     * @param relationType Relation type.
     */
    explicit UMLRelation(UMLClassifier *src, UMLClassifier *dst, UMLRelationType relationType);

    /**
     * @brief Returns relation source class element.
     * @return Source class element.
     */
    const UMLClassifier *src() const;

    /**
     * @brief Returns relation destination class element.
     * @return Destination class element.
     */
    const UMLClassifier *dst() const;

    /**
     * @brief Updates relation type.
     * @param relationType New relation type.
     */
    void setRelationType(const UMLRelationType &relationType);

    /**
     * @brief Returns relation type.
     * @return Relation type.
     */
    const UMLRelationType &relationType() const;

    /**
     * @brief Returns relation content message.
     * @return Relation content text.
     */
    std::string &relationMsg();

    /**
     * @brief Returns relation content message.
     * @return Relation content text.
     */
    const std::string &relationMsg() const;

    /**
     * @brief Returns source message.
     * @return Source message.
     */
    std::string &srcMsg();

    /**
     * @brief Returns source message.
     * @return Source message.
     */
    const std::string &srcMsg() const;

    /**
     * @brief Returns destination message.
     * @return Destination message.
     */
    std::string &dstMsg();

    /**
     * @brief Returns destination message.
     * @return Destination message.
     */
    const std::string &dstMsg() const;

    /**
     * @brief Adds this relation to both source and destination classes.
     */
    void setRelationDependency();

    /**
     * @brief Swaps source and destination classes.
     */
    void swapDirection();

    /**
     * @brief Removes this relation from second class.
     * @param src Class of origin (second class will be edited).
     */
    void removeRelationDependency(UMLClassifier *src);

    /**
     * @brief Compare if this relation is between two given classes.
     * @param cls1 First class.
     * @param cls2 Second class.
     * @return
     */
    bool compareClassesInRelation(const UMLClassifier *cls1, const UMLClassifier *cls2);

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createObject(QJsonObject &object);

    ~UMLRelation() =default;
};

#endif //DIAGRAMEDITOR_UMLRELATION_H
