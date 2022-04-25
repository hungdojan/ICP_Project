#include "UMLObject.h"
#include <QString>

const UMLClass *UMLObject::model() const {
    return model_;
}

void UMLObject::setModel(UMLClass *model) {
    model_ = model;
}

void UMLObject::createJsonObject(QJsonObject &object) {
    object.insert("_class", "UMLObject");
    object.insert("name", QString::fromStdString(name_));
    object.insert("model", QString::fromStdString(model_->name()));
}
