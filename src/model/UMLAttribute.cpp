//
// Created by rebulien on 4/19/22.
//

#include "UMLAttribute.h"
#include <sstream>
#include <stdexcept>
#include <QString>

void UMLAttribute::setName(const std::string &name) {
    Element::setName(name);
}

char &UMLAttribute::visibility() {
    return visibility_;
}

const UMLClassifier *UMLAttribute::type() const {
    return type_;
}

UMLClassifier *UMLAttribute::setType(UMLClassifier *classifier) {
    if (classifier == nullptr)
        throw std::invalid_argument("Given parameter cannot be nullptr");

    auto temp = type_;
    type_ = classifier;
    return temp;
}

bool UMLAttribute::operator==(UMLAttribute &attr) const {
    return type_ == attr.type_ && name_ == attr.name_;
}

UMLAttribute::operator std::string() const {
    std::ostringstream stream;
    stream << visibility_ << type_->name() << " " << name_;
    return stream.str();
}

std::ostream &operator<<(std::ostream &strm, const UMLAttribute &attr) {
    std::string str{attr};
    return strm << str;
}

UMLAttribute::~UMLAttribute() {
    if (!type_->isUserDefined())
        delete type_;
}

void UMLAttribute::createJsonObject(QJsonObject &object) {
    object.insert("_class", "UMLAttribute");
    object.insert("name", QString::fromStdString(name_));
    object.insert("type", QString::fromStdString(type_->name()));
    object.insert("visibility", visibility_);
}
