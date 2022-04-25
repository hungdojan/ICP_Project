//
// Created by rebulien on 4/19/22.
//

#include "UMLAttribute.h"
#include "ClassDiagram.h"
#include <sstream>
#include <stdexcept>
#include <QString>

UMLAttribute::UMLAttribute(const std::string &name, UMLClassifier *classifier) : Element(name), type_{classifier} {
    if (classifier != nullptr)
        classifier->attach(this);
}

void UMLAttribute::setName(const std::string &name) {
    Element::setName(name);
}

char &UMLAttribute::visibility() {
    return visibility_;
}

const UMLClassifier *UMLAttribute::type() const {
    return type_;
}

const UMLClassifier *UMLAttribute::setType(UMLClassifier *classifier) {
    if (classifier == nullptr)
        throw std::invalid_argument("Given parameter cannot be nullptr");

    if (type_ != ClassDiagram::undefClassifier_) {
        type_->detach(this);
    }
    auto temp = type_;
    type_ = classifier;
    if (type_ != ClassDiagram::undefClassifier_) {
        type_->attach(this);
    }
    return temp;
}

bool UMLAttribute::operator==(UMLAttribute &attr) const {
    return type_ == attr.type_ && name_ == attr.name_;
}

UMLAttribute::operator std::string() const {
    std::ostringstream stream;
    if (visibility_ != 0)
        stream << visibility_;
    stream << type_->name() << " " << name_;
    return stream.str();
}

std::ostream &operator<<(std::ostream &strm, const UMLAttribute &attr) {
    std::string str{attr};
    return strm << str;
}


void UMLAttribute::update(const std::string &msg) {
    if (msg == "DELETE") {
        type_ = ClassDiagram::undefClassifier_;
    }
}

UMLAttribute::~UMLAttribute() {
    // if (!type_->isUserDefined())
    //     delete type_;
}

void UMLAttribute::createJsonObject(QJsonObject &object) {
    object.insert("_class", "UMLAttribute");
    object.insert("name", QString::fromStdString(name_));
    object.insert("type", QString::fromStdString(type_->name()));
    object.insert("visibility", visibility_);
}
