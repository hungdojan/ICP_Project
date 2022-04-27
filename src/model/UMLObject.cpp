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
