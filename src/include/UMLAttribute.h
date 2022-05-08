/**
 * @brief Declaration of UMLAttribute.
 * UMLAttribute represents abstract structure of attribute of class element in the class diagram.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLAttribute.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_UMLATTRIBUTE_H
#define DIAGRAMEDITOR_UMLATTRIBUTE_H
#include "Element.h"
#include "UMLClassifier.h"

/**
 * @brief Representation of attribute in class diagram.
 */
class UMLAttribute : public Element, public IObserver {
protected:
    /** Visibility character */
    char visibility_{};
    /** Attributes type */
    UMLClassifier *type_;
public:
    UMLAttribute() =delete;
    /**
     * @brief Class constructor.
     * @param name Attribute name.
     * @param classifier Attribute's type.
     */
    explicit UMLAttribute(const std::string &name, UMLClassifier *classifier);

    /**
     * @brief Updates attribute's name.
     * @param name Attribute name.
     */
    void setName(const std::string &name) override;

    /**
     * @brief Return attribute's visibility.
     * @return Attribute's visibility.
     */
    char &visibility();

    /**
     * @brief Returns attribute's type.
     * @return Attribute's type.
     */
    const UMLClassifier *type() const;

    /**
     * @brief Updates attribute's type.
     * @param classifier New attribute's type.
     * @return Attribute's old type.
     */
    const UMLClassifier *setType(UMLClassifier *classifier);

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createJsonObject(QJsonObject &object) override;

    /**
     * @brief Message receiver method.
     * @param msg Message content.
     */
    void update(const std::string &msg) override;

    /**
     * @brief Overrides operator ==
     * @param attr Reference of attribute.
     * @return Bool output value.
     */
    bool operator==(UMLAttribute &attr) const;


    /**
     * @brief Overrides operator std::string.
     * @return New string value.
     */
    virtual explicit operator std::string() const;

    /**
     * @brief Override stream operator.
     * @param strm Reference to the stream.
     * @param operation Reference to the operation.
     * @return Updated stream.
     */
    friend std::ostream& operator<<(std::ostream &strm, const UMLAttribute &attr);

    /**
     * @brief Class destructor.
     */
    ~UMLAttribute() override;
};


#endif //DIAGRAMEDITOR_UMLATTRIBUTE_H
