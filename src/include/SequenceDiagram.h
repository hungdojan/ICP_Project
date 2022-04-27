/**
 * @brief Declaration of SequenceDiagram class.
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

class SequenceDiagram : public Element {
    std::vector<UMLObject *> objects_;
    std::vector<UMLMessage *> messages_;
    ClassDiagram &model_;
public:
    static UMLClass *undefClass;
    static UMLOperation *undefOperation;
    static UMLObject *undefObject;
    SequenceDiagram() =delete;
    explicit SequenceDiagram(const std::string &name, ClassDiagram &model) : Element{name}, model_{model} { }
    const std::vector<UMLObject *> &objects() const;
    const std::vector<UMLMessage *> &messages() const;
    const ClassDiagram &model() const;

    UMLObject *addObject(UMLClass *instanceOfClass, const std::string &objName);
    UMLObject *getObject(const std::string &name) const;
    bool removeObject(const std::string &name);

    UMLMessage *addMessage(UMLObject *src, UMLObject *dst);
    bool addMessage(UMLMessage *msg);
    UMLMessage *getMessageAt(int pos);
    long getMessagePosition(UMLMessage *msg);
    bool moveMessageIntoPosition(UMLMessage *msg, int pos);
    bool removeMessage(UMLMessage *msg);
    bool removeMessage(int pos);

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createJsonObject(QJsonObject &object) override;
    ~SequenceDiagram() override;
};


#endif //DIAGRAMEDITOR_SEQUENCEDIAGRAM_H
