/**
 * @brief Declaration of SequenceDiagram class.
 * Sequence diagram displays interaction between objects through messages. This class holds these information of these
 * elements and lists some methods that works with elements.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22
 *
 * @file SequenceDiagram.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */
#ifndef DIAGRAMEDITOR_SEQUENCEDIAGRAM_H
#define DIAGRAMEDITOR_SEQUENCEDIAGRAM_H
#include <utility>
#include <vector>
#include "Element.h"
#include "UMLClass.h"
#include "UMLObject.h"
#include "UMLMessage.h"

class ClassDiagram;

/**
 * @brief Representation of sequence diagram in UML editor.
 */
class SequenceDiagram : public Element {
    /** Collection of objects. */
    std::vector<UMLObject *> objects_;
    /** Collection of messages. */
    std::vector<UMLMessage *> messages_;
    /** Base class diagram model */
    ClassDiagram &model_;
public:
    /** Instance of undefined class */
    static UMLClass *undefClass;
    /** Instance of undefined operation */
    static UMLOperation *undefOperation;
    /** Instance of undefined object */
    static UMLObject *undefObject;
    SequenceDiagram() =delete;

    /**
     * @brief Class constructor.
     * @param name Diagram's name.
     * @param model Diagram's class diagram model.
     */
    explicit SequenceDiagram(const std::string &name, ClassDiagram &model) : Element{name}, model_{model} { }

    /**
     * @brief Returns collection of objects in the diagram.
     * @return Vector of objects.
     */
    const std::vector<UMLObject *> &objects() const;

    /**
     * @brief Returns collection of messages in the diagram.
     * @return Vector of messages.
     */
    const std::vector<UMLMessage *> &messages() const;

    /**
     * @brief Returns diagram's class diagram model.
     * @return Diagram's class diagram model.
     */
    const ClassDiagram &model() const;

    /**
     * @brief Create and insert instance of UMLObject into the diagram.
     * If object with same name already exists, insertion is halted.
     * @param instanceOfClass Object's class model.
     * @param objName Object's name.
     * @return Instance of created and added object, nullptr otherwise.
     */
    UMLObject *addObject(UMLClass *instanceOfClass, const std::string &objName);

    /**
     * @brief Returns instance of object with a given name.
     * @param name Object's name.
     * @return Instance of found object, nullptr if not found.
     */
    UMLObject *getObject(const std::string &name) const;

    /**
     * @brief Returns instance of object with a given name.
     * @param name Object's name.
     * @return Instance of found object, nullptr if not found.
     */
    UMLObject *getObject(QString name) const;

    /**
     * @brief Removes object from the diagram if found.
     * @param name Object's name.
     * @return Whether removal ended successfully.
     */
    bool removeObject(const std::string &name);

    /**
     * @brief Create and insert instance of UMLMessage into the diagram.
     * @param src Source of message (sender).
     * @param dst Destination of message (receiver).
     * @return Instance of created and inserted message.
     */
    UMLMessage *addMessage(UMLObject *src, UMLObject *dst);

    /**
     * @brief Insert instance of message into the diagram.
     * @param msg Instance of message.
     * @return Whether insertion ended successfully.
     */
    bool addMessage(UMLMessage *msg);

    /**
     * @brief Returns message at a given position.
     * @param pos Message's position.
     * @return Instance of found message, nullptr if not found.
     */
    UMLMessage *getMessageAt(int pos);

    /**
     * @brief Returns message's position within the diagram.
     * @param msg Instance of message.
     * @return Message's position, -1 if not found.
     */
    long getMessagePosition(UMLMessage *msg);

    /**
     * @brief Removes message from the diagram.
     * @param msg Instance of message.
     * @return Whether removal ended successfully.
     */
    bool removeMessage(UMLMessage *msg);

    /**
     * @brief Removes message at a given position from the diagram.
     * @param pos Message's position.
     * @return Whether removal ended successfully.
     */
    bool removeMessage(int pos);

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createJsonObject(QJsonObject &object) override;

    /**
     * @brief Class desturctor.
     */
    ~SequenceDiagram() override;
};

#endif //DIAGRAMEDITOR_SEQUENCEDIAGRAM_H
