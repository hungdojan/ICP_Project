/**
 * @brief Definition of class UMLRelation methods.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file UMLRelation.cpp
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "UMLRelation.h"
#include "UMLInterface.h"
#include <algorithm>
#include <stdexcept>
#include <QString>

UMLRelation::UMLRelation(UMLClassifier *src, UMLClassifier *dst)
        : src_{src}, dst_{dst}, relationType_{ASSOCIATION}, srcMsg_{}, dstMsg_{} {
    setRelationDependency();
}

UMLRelation::UMLRelation(UMLClassifier *src, UMLClassifier *dst, UMLRelationType relationType)
        : src_{src}, dst_{dst}, relationType_{relationType}, srcMsg_{}, dstMsg_{} {
    setRelationDependency();
}

const UMLClassifier *UMLRelation::src() const {
    return src_;
}

const UMLClassifier *UMLRelation::dst() const {
    return dst_;
}

std::string &UMLRelation::relationMsg() {
    return relationMsg_;
}
const std::string &UMLRelation::relationMsg() const {
    return relationMsg_;
}

std::string &UMLRelation::srcMsg() {
    return srcMsg_;
}

const std::string &UMLRelation::srcMsg() const {
    return srcMsg_;
}

std::string &UMLRelation::dstMsg() {
    return dstMsg_;
}

const std::string &UMLRelation::dstMsg() const {
    return dstMsg_;
}

void UMLRelation::setRelationType(const UMLRelation::UMLRelationType &relationType) {
    if (relationType_ == INHERITANCE) {
        src_->removeParentClass(dst_);
    }
    relationType_ = relationType;
    if (relationType_ == INHERITANCE) {
        if (dynamic_cast<UMLInterface *>(src_) != nullptr && dynamic_cast<UMLInterface *>(dst_) == nullptr) {
            throw std::invalid_argument("Interface cannot derive from non-interface element");
        }
        else if (src_ == dst_) {
            throw std::invalid_argument("Class element cannot derive from itself.");
        }
        src_->addParentClass(dst_);
    }
}

const UMLRelation::UMLRelationType &UMLRelation::relationType() const {
    return relationType_;
}

void UMLRelation::setRelationDependency() {
    if (relationType_ == INHERITANCE) {
        if (dynamic_cast<UMLInterface *>(src_) != nullptr && dynamic_cast<UMLInterface *>(dst_) == nullptr) {
            throw std::invalid_argument("Interface cannot derive from non-interface element");
        }
        else if (src_ == dst_) {
            throw std::invalid_argument("Class element cannot derive from itself.");
        }
        src_->addParentClass(dst_);
    }
    src_->relations_.insert(this);
    dst_->relations_.insert(this);
}

void UMLRelation::swapDirection() {
    if (src_ == dst_)
        return;
    if (relationType_ == INHERITANCE) {
        if (dynamic_cast<UMLInterface *>(dst_) != nullptr && dynamic_cast<UMLInterface *>(src_) == nullptr) {
            throw std::invalid_argument("Interface cannot derive from non-interface element");
        }
        src_->removeParentClass(dst_);
        dst_->addParentClass(src_);
    }
    auto tmp = dst_;
    dst_ = src_;
    src_ = tmp;
}


void UMLRelation::removeRelationDependency(UMLClassifier *src) {
    if (relationType_ == INHERITANCE) {
        src_->removeParentClass(dst_);
    }
    if (src_ != src && dst_ != src)
        throw std::invalid_argument("Parameter src MUST be instance of src_ or dst_");
    if (src_ == dst_)
        return;

    if (src == src_) {
        auto dstRelation{std::find(dst_->relations_.begin(), dst_->relations_.end(), this)};
        if (dstRelation != dst_->relations_.end())
            dst_->relations_.erase(this);
    }
    else {
        auto srcRelation{std::find(src_->relations_.begin(), src_->relations_.end(), this)};
        if (srcRelation != src_->relations_.end())
            src_->relations_.erase(this);
    }
}

bool UMLRelation::compareClassesInRelation(const UMLClassifier *cls1, const UMLClassifier *cls2) {
    return (cls1 == src_ && cls2 == dst_) || (cls2 == src_ && cls1 == dst_);
}

void UMLRelation::createObject(QJsonObject &object) {
    object.insert("_class", "UMLRelation");
    object.insert("src", QString::fromStdString(src_->name()));
    object.insert("dst", QString::fromStdString(dst_->name()));
    object.insert("relationType", relationType_);
    object.insert("dstMsg", QString::fromStdString(dstMsg_));
    object.insert("srcMsg", QString::fromStdString(srcMsg_));
}
