//
// Created by rebulien on 4/19/22.
//

#include "SequenceDiagram.h"
#include "UMLMessage.h"
#include <algorithm>
#include <QString>
#include <QJsonArray>

UMLClass *SequenceDiagram::undefClass = new UMLClass("#UNDEF", {});
UMLOperation *SequenceDiagram::undefOperation = new UMLOperation("#UNDEF", SequenceDiagram::undefClass);
UMLObject *SequenceDiagram::undefObject = new UMLObject("#UNDEF", SequenceDiagram::undefClass);

UMLObject *SequenceDiagram::addObject(UMLClass *instanceOfClass, const std::string &objName) {
    auto iter {std::find_if(objects_.begin(), objects_.end(),
                            [objName](UMLObject *obj) { return objName == obj->name(); })};
    // TODO: duplicate found
    if (iter != objects_.end())
        return nullptr;
    auto obj = new UMLObject(objName, instanceOfClass);
    objects_.push_back(obj);
    return obj;
}

const std::vector<UMLObject *> &SequenceDiagram::objects() const {
    return objects_;
}

const std::vector<UMLMessage *> &SequenceDiagram::messages() const {
    return messages_;
}

void SequenceDiagram::createJsonObject(QJsonObject &object) {
    object.insert("_class", "SequenceDiagram");
    object.insert("name", QString::fromStdString(name_));

    // insert objects
    QJsonArray qObjects;
    for (auto o : objects_) {
        QJsonObject qObject;
        o->createJsonObject(qObject);
        qObjects.push_back(qObject);
    }
    object.insert("objects", qObjects);

    // insert messages
    QJsonArray qMessages;
    int i = 0;
    for (auto m = messages_.begin(); m != messages_.end(); ++i, ++m) {
        QJsonObject qMessage;
        (*m)->createJsonObject(qMessage, i);
        qMessages.push_back(qMessage);
    }
    object.insert("messages", qMessages);
}

const ClassDiagram &SequenceDiagram::model() const {
    return model_;
}

UMLObject *SequenceDiagram::getObject(const std::string &name) const {
    auto iter{std::find_if(objects_.begin(), objects_.end(),
                           [name](UMLObject *obj) { return name == obj->name(); })};
    if (iter == objects_.end())
        return nullptr;
    return *iter;
}

bool SequenceDiagram::removeObject(const std::string &name) {
    auto iter{std::find_if(objects_.begin(), objects_.end(),
                           [name](UMLObject *obj) { return name == obj->name(); })};
    if (iter == objects_.end())
        return false;
    auto found = *iter;
    objects_.erase(iter);
    delete found;
    return true;
}

SequenceDiagram::~SequenceDiagram() {
    for (auto o : objects_) {
        delete o;
    }
    for (auto m : messages_) {
        delete m;
    }
    // delete SequenceDiagram::undefOperation;
    // delete SequenceDiagram::undefClass;
}

UMLMessage *SequenceDiagram::addMessage(UMLObject *src, UMLObject *dst) {
    auto newMessage = new UMLMessage(nullptr, src, dst);
    messages_.push_back(newMessage);
    return newMessage;
}

UMLMessage *SequenceDiagram::getMessageAt(int pos) {
    if (pos < 0 || pos >= messages_.size())
        return nullptr;
    return messages_.at(pos);
}

long SequenceDiagram::getMessagePosition(UMLMessage *msg) {
    auto iter{std::find(messages_.begin(), messages_.end(), msg)};
    if (iter == messages_.end())
        return -1;
    return std::distance(iter, messages_.end());
}

bool SequenceDiagram::moveMessageIntoPosition(UMLMessage *msg, int pos) {
    if (pos < 0 || pos >= messages_.size())
        return false;
    auto iter{std::find(messages_.begin(), messages_.end(),msg)};
    if (iter == messages_.end())
        return false;
    auto item = *iter;
    messages_.erase(iter);
    iter = messages_.begin();
    iter += pos;
    messages_.insert(iter, item);
    return true;
}

bool SequenceDiagram::removeMessage(UMLMessage *msg) {
    auto iter{std::find(messages_.begin(), messages_.end(), msg)};
    if (iter == messages_.end())
        return false;
    messages_.erase(iter);
    return true;
}

bool SequenceDiagram::removeMessage(int pos) {
    if (pos < 0 || pos >= messages_.size())
        return false;
    auto found = messages_.at(pos);
    messages_.erase(messages_.begin() + pos);
    delete found;
    return true;
}

bool SequenceDiagram::addMessage(UMLMessage *msg) {
    auto iter{std::find(messages_.begin(), messages_.end(), msg)};
    if (iter != messages_.end())
        return false;
    messages_.push_back(msg);
    return true;
}


