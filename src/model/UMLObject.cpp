/**
 * @brief Definition of UMLObject class methods.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLObject.cpp
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "UMLObject.h"
#include "ClassDiagram.h"
#include <QString>

UMLClass *UMLObject::model() const {
    return model_;
}

void UMLObject::setModel(UMLClass *model) {
    if (model_ != SequenceDiagram::undefClass) {
        model_->detach(this);
    }
    model_ = model;
    if (model_ != SequenceDiagram::undefClass) {
        model_->attach(this);
    }
}

void UMLObject::createJsonObject(QJsonObject &object) {
    object.insert("_class", "UMLObject");
    object.insert("name", QString::fromStdString(name_));
    object.insert("model", QString::fromStdString(model_->name()));
}

void UMLObject::update(const std::string &msg) {
    if (msg == "DELETE") {
        model_ = SequenceDiagram::undefClass;
    }
}
