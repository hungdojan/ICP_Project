/**
 * @brief Definition of UMLAttribute methods.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLAttribute.cpp
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "UMLAttribute.h"
#include "ClassDiagram.h"
#include <sstream>
#include <stdexcept>
#include <QString>

UMLAttribute::UMLAttribute(const std::string &name, UMLClassifier *classifier) : Element(name), type_{classifier} {
    if (classifier != nullptr)
        classifier->attach(this);
    else
        type_ = ClassDiagram::undefClassifier_;
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
    if (type_ != nullptr)
        type_->detach(this);
}

void UMLAttribute::createJsonObject(QJsonObject &object) {
    object.insert("_class", "UMLAttribute");
    object.insert("name", QString::fromStdString(name_));
    object.insert("type", QString::fromStdString(type_->name()));
    object.insert("visibility", visibility_);
}
