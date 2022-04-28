#include "SequenceDiagram.h"
#include "UMLMessage.h"
#include "UMLClass.h"
#include <algorithm>
#include <stdexcept>
#include <QString>

UMLMessage::UMLMessage(UMLOperation *baseOperation, UMLObject *src, UMLObject *dst)
    : Element{""}, src_{src},dst_{dst}, baseOperation_{nullptr} {
    if (src == nullptr && dst == nullptr)
        throw std::invalid_argument("At least one of the objects must not be nullptr");
    if (src == nullptr)
        src_ = SequenceDiagram::undefObject;
    if (dst == nullptr)
        dst_ = SequenceDiagram::undefObject;
    setOperation(baseOperation);
}
const UMLObject *UMLMessage::dst() const {
    return dst_;
}

const UMLObject *UMLMessage::src() const {
    return src_;
}

const UMLOperation *UMLMessage::baseOperation() const {
    return baseOperation_;
}

bool UMLMessage::setOperation(const std::string &name) {
    auto operationsAvailable = dst_->model()->getOperations();
    auto iter{std::find_if(operationsAvailable.begin(), operationsAvailable.end(),
                           [name](UMLOperation *o) { return o->name() == name; })};
    if (iter == operationsAvailable.end())
        return false;
    if (baseOperation_ != nullptr)
        baseOperation_->detach(this);
    baseOperation_ = *iter;
    if (baseOperation_ != nullptr)
        baseOperation_->attach(this);
    return true;
}

bool UMLMessage::setOperation(UMLOperation *operation) {
    if (operation == nullptr) {
        baseOperation_ = SequenceDiagram::undefOperation;
        return true;
    }
    else if (operation == SequenceDiagram::undefOperation) {
        baseOperation_ = operation;
        return true;
    }

    auto operationsAvailable = dst_->model()->getOperations();
    auto iter{operationsAvailable.find(operation)};
    if (iter == operationsAvailable.end())
        return false;
    baseOperation_ = *iter;
    baseOperation_->attach(this);
    return true;
}

UMLMessage::UMLMessage(const std::string &name, UMLObject *src, UMLObject *dst)
    : Element(name), src_{src}, dst_{dst}, baseOperation_{SequenceDiagram::undefOperation} {
}

std::string UMLMessage::text() const {
    if (!name_.empty()) {
        return name_;
    }
    else if (baseOperation_ != nullptr) {
        return baseOperation_->name();
    }
    else {
        return "";
    }
}

void UMLMessage::setText(const std::string &text) {
    Element::setName(text);
}

void UMLMessage::update(const std::string &msg) {
    if (msg == "DELETE") {
        baseOperation_ = SequenceDiagram::undefOperation;
    }
}

void UMLMessage::createJsonObject(QJsonObject &object, int index) {
    createJsonObject(object);
    object.insert("index", index);
}

void UMLMessage::createJsonObject(QJsonObject &object) {
    object.insert("_class", "UMLMessage");
    object.insert("src", QString::fromStdString(src_->name()));
    object.insert("dst", QString::fromStdString(dst_->name()));
    object.insert("name", QString::fromStdString(name_));
    object.insert("baseOperation", QString::fromStdString(baseOperation_->name()));

}

UMLMessage::~UMLMessage() {
    if (baseOperation_ != nullptr) {
        baseOperation_->detach(this);
    }
}
