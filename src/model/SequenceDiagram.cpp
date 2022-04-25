//
// Created by rebulien on 4/19/22.
//

#include "SequenceDiagram.h"
#include "UMLMessage.h"
#include <algorithm>
#include <QString>
#include <QJsonArray>

bool SequenceDiagram::addObject(UMLClass *instanceOfClass, const std::string &objName) {
    auto iter {std::find_if(objects_.begin(), objects_.end(),
                            [objName](UMLObject &obj) { return objName == obj.name(); })};
    // TODO: duplicate found
    if (iter != objects_.end())
        return false;
    objects_.emplace_back(objName, instanceOfClass);
    return true;
}

const std::vector<UMLObject> &SequenceDiagram::objects() const {
    return objects_;
}

const std::vector<UMLMessage> &SequenceDiagram::messages() const {
    return messages_;
}

// UMLObject &SequenceDiagram::getObject(const std::string &name) const {
//     auto iter{std::find_if(objects_.begin(), objects_.end(),
//                            [name](UMLObject *o) { return name == o->model()->name(); })};
//     if (iter == objects_.end())
//         return nullptr;
//     return *iter;
// }
//
// bool SequenceDiagram::removeObject(const std::string &name) {
//     auto iter{std::find_if(objects_.begin(), objects_.end(),
//                            [name](UMLObject *o) { return name == o->model()->name(); })};
//     if (iter == objects_.end())
//         return false;
//     objects_.erase(iter);
//     delete *iter;
//     return true;
// }
//
// SequenceDiagram::~SequenceDiagram() {
//     for (auto o : objects_) {
//         delete o;
//     }
// }

void SequenceDiagram::createJsonObject(QJsonObject &object) {
    object.insert("_class", "SequenceDiagram");
    object.insert("name", QString::fromStdString(name_));

    // insert objects
    QJsonArray qObjects;
    for (auto o : objects_) {
        QJsonObject qObject;
        o.createJsonObject(qObject);
        qObjects.push_back(qObject);
    }
    object.insert("objects", qObjects);

    // insert messages
    QJsonArray qMessages;
    for (auto m : messages_) {
        QJsonObject qMessage;
        m.createJsonObject(qMessage);
        qMessages.push_back(qMessage);
    }
    object.insert("messages", qMessages);
}

const ClassDiagram &SequenceDiagram::model() const {
    return model_;
}

const UMLObject *SequenceDiagram::getObject(const std::string &name) const {
    auto iter{std::find_if(objects_.begin(), objects_.end(),
                           [name](const UMLObject &obj) { return name == obj.name(); })};
    if (iter == objects_.end())
        return nullptr;
    return &(*iter);
}

bool SequenceDiagram::removeObject(const std::string &name) {
    auto iter{std::find_if(objects_.begin(), objects_.end(),
                           [name](const UMLObject &obj) { return name == obj.name(); })};
    if (iter == objects_.end())
        return false;
    objects_.erase(iter);
    return true;
}

bool SequenceDiagram::addMessage(UMLObject &src, UMLObject &dst) {
    auto iter{std::find_if(messages_.begin(), messages_.end(),
              [src,dst](const UMLMessage &m) { return m.src().name() == src.name() && m.dst().name() == dst.name(); })};
    if (iter != messages_.end())
        return false;
    messages_.emplace_back(nullptr, src, dst);
    return true;
}



SequenceDiagram::~SequenceDiagram() =default;
