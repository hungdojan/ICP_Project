/**
 * @brief Declaration of abstract class UMLOperation.
 * UMLOperation represents abstract structure of class element's operation. Operation holds indefinite number of parameters.
 * Every parameter must have an unique name within its owner.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLOperation.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_UMLOPERATION_H
#define DIAGRAMEDITOR_UMLOPERATION_H
#include <vector>
#include "Element.h"
#include "UMLAttribute.h"
#include "IObserver.h"

/**
 * @brief Representation of operation in class diagram.
 */
class UMLOperation : public UMLAttribute, public ISubject {
protected:
    /** Collection of parameters. */
    std::vector<UMLAttribute*> parameters_;
public:
    UMLOperation() =delete;

    /**
     * @brief Class constructor.
     * @param name Operation name.
     * @param type Operation return type.
     */
    UMLOperation(const std::string &name, UMLClassifier *type) : UMLAttribute(name, type) { }

    /**
     * @brief Class constructor.
     * @param name Operation name.
     * @param type Operation return type.
     * @param parameters Collection of parameters.
     */
    UMLOperation(const std::string &name, UMLClassifier *type, const std::vector<UMLAttribute*> &parameters);

    /**
     * @brief Returns collection of parameters.
     * @return Vector of parameters.
     */
    const std::vector<UMLAttribute*> &parameters() const;

    /**
     * @brief Adds new parameter to the operation.
     * If parameter with identical name already exists in the operation
     * OLD instance IS KEPT.
     * @param parameter New instance of parameter.
     * @return Whether insertion was successful.
     */
    bool addParameter(UMLAttribute *parameter);

    /**
     * @brief Adds new parameter to the operation.
     * If parameter with identical name already exists in the operation
     * OLD instance IS REMOVED and REPLACE with new one.
     * @param parameter New instance of parameter.
     * @return Old instance of parameter if collision found, nullptr otherwise.
     */
    UMLAttribute *addOrReplaceParameter(UMLAttribute *parameter);

    /**
     * @brief Returns parameter with given name.
     * @param name Parameter's name.
     * @return Found instance of parameter, nullptr otherwise.
     */
    UMLAttribute *getParameterByName(const std::string &name) const;

    /**
     * @brief Removes parameter with given name.
     * @param name Parameter's name.
     * @return Whether removal ended successfully.
     */
    bool removeParameter(const std::string &name);

    /**
     * @brief Removes instance of parameter from the operation.
     * @param parameter Instance of parameter.
     * @return Whether removal ended successfully.
     */
    bool removeParameter(UMLAttribute *parameter);

    /**
     * @brief Removes all parameters from the operation.
     * @param vectorContent Output vector to pass all parameters back to user.
     */
    void clearParameters(std::vector<UMLAttribute *> *vectorContent);

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
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createJsonObject(QJsonObject &object) override;

    /**
     * @brief Overrides operator std::string.
     * @return New string value.
     */
    explicit operator std::string() const final;

    /**
     * @brief Override stream operator.
     * @param strm Reference to the stream.
     * @param operation Reference to the operation.
     * @return Updated stream.
     */
    friend std::ostream &operator<<(std::ostream &strm, const UMLOperation &operation);

    /**
     * Class destructor.
     */
    ~UMLOperation() override;
};


#endif //DIAGRAMEDITOR_UMLOPERATION_H
