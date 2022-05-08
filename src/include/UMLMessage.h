/**
 * @brief Declaration of class UMLMessage.
 * UMLMessage represents abstract structure of communication between two objects in the sequence diagram.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLMessage.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_UMLMESSAGE
#define DIAGRAMEDITOR_UMLMESSAGE
#include "Element.h"
#include "UMLOperation.h"
#include "UMLObject.h"

/**
 * @brief Representation of communication in sequence diagram.
 */
class UMLMessage : public Element, public IObserver {
    /** Source object (sender) */
    UMLObject *src_;
    /** Destination object (receiver) */
    UMLObject *dst_;
    /** Operation of communication */
    UMLOperation *baseOperation_;
    /** Type of communication */
    std::string messageType_;
public:
    UMLMessage() =delete;
    /**
     * @brief Class constructor.
     * @param baseOperation Message's operation model.
     * @param src Source object.
     * @param dst Destination object.
     */
    explicit UMLMessage(UMLOperation *baseOperation, UMLObject *src, UMLObject *dst);

    /**
     * @brief Class constructor.
     * @param name Message name (content).
     * @param src Source object.
     * @param dst Destination object.
     */
    explicit UMLMessage(const std::string &name, UMLObject *src, UMLObject *dst);

    /**
     * @brief Returns message text to display.
     * @return Text to display.
     */
    std::string text() const;

    /**
     * @brief Updates text to display.
     * @param text New text.
     */
    void setText(const std::string &text);

    /**
     * @brief Returns message type.
     * @return Message type.
     */
    const std::string &messageType() const;

    /**
     * @brief Returns message type.
     * @return Message type.
     */
    std::string &messageType();

    /**
     * @brief Returns destination of message.
     * @return Destination object.
     */
    const UMLObject *dst() const;

    /**
     * @brief Returns source of message.
     * @return Source object.
     */
    const UMLObject *src() const;

    /**
     * @brief Returns base operation of message.
     * @return Operation of message.
     */
    const UMLOperation *baseOperation() const;

    /**
     * @brief Updates message of new operation.
     * @param operation Instance of new operation.
     * @return Whether update ended successfully.
     */
    bool setOperation(UMLOperation *operation);

    /**
     * @brief Updates message of new operation.
     * @param operationName Name of destinations class model's operation.
     * @return Whether update ended successfully.
     */
    bool setOperation(const std::string &operationName);

    /**
     * @brief Message receiver method.
     * @param msg Message content.
     */
    void update(const std::string& msg) override;

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createJsonObject(QJsonObject &object) override;

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     * @param index Position of message.
     */
    void createJsonObject(QJsonObject &object, int index);

    /**
     * @brief Class destructor.
     */
    ~UMLMessage() override;
};
#endif //DIAGRAMEDITOR_UMLMESSAGE