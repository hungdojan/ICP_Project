#include "UMLInterface.h"
#include "UMLRelation.h"
#include <algorithm>
#include <QJsonArray>

UMLInterface::UMLInterface(const std::string &name) : UMLClassifier(name, true) {
    isAbstract_ = true;
}

UMLInterface::UMLInterface(const std::string &name, const std::vector<UMLOperation *> &operations)
        : UMLClassifier(name, true) {
    isAbstract_ = true;
    for (auto operation : operations) {
        addOperation(operation);
    }
}

bool UMLInterface::addOperation(UMLOperation *operation) {
    if (operation == nullptr)
        return false;
    // TODO: add instance with identical content
    auto iter{std::find_if(operations_.begin(), operations_.end(),
                           [operation](const UMLOperation *oper) { return operation->name() == oper->name(); })};
    if (iter != operations_.end())
        return false;
    operations_.push_back(operation);
    return true;
}

UMLOperation *UMLInterface::getOperation(const std::string &name) const {
    auto iter{std::find_if(operations_.begin(), operations_.end(),
                           [name](const UMLOperation *oper) { return name == oper->name(); })};
    if (iter == operations_.end())
        return nullptr;
    return *iter;
}

bool UMLInterface::containsOperation(const UMLOperation *operation) const {
    if (operation == nullptr)
        return false;
    auto iter{std::find(operations_.begin(), operations_.end(), operation)};
    return iter == operations_.end();
}

const std::vector<UMLOperation *> &UMLInterface::operations() const {
    return operations_;
}

bool UMLInterface::removeOperation(UMLOperation *operation) {
    if (operation == nullptr)
        return false;
    auto iter{std::find(operations_.begin(), operations_.end(),operation)};
    if (iter == operations_.end())
        return false;
    operations_.erase(iter);
    return true;
}

void UMLInterface::addOperations(const std::vector<UMLOperation *> &operations) {
    for (auto o : operations) {
        addOperation(o);
    }
}

UMLRelation *UMLInterface::addRelation(UMLClassifier *dst) {
    if (inRelationWith(dst))
        return nullptr;
    return new UMLRelation(this, dst);
}

bool UMLInterface::removeRelation(UMLRelation *relation) {
    auto iter{std::find(relations_.begin(), relations_.end(), relation)};
    if (iter == relations_.end())
        return false;
    relations_.erase(iter);
    relation->removeRelationDependency(this);
    delete relation;
    return true;
}

bool UMLInterface::removeRelation(UMLClassifier *dstClass) {
    if (!inRelationWith(dstClass))
        return false;
    auto iter{std::find_if(relations_.begin(), relations_.end(),
                           [&,dstClass](UMLRelation *r) { return r->compareClassesInRelation(this, dstClass); })};
    if (iter == relations_.end())
        return false;
    (*iter)->removeRelationDependency(this);
    relations_.erase(iter);
    delete *iter;
    return true;
}

UMLInterface::~UMLInterface() {
    for (auto o : operations_)
        delete o;
}

void UMLInterface::createObject(QJsonObject &object) {
    object.insert("_class", "UMLInterface");
    object.insert("name", QString::fromStdString(name_));
    object.insert("isUserDefined", isUserDefined_);
    object.insert("isAbstract", isAbstract_);
    QJsonArray lofOperations;
    for (auto o : operations_) {
        QJsonObject obj;
        o->createObject(obj);
        lofOperations.push_back(obj);
    }
    object.insert("operations", lofOperations);
}

std::unordered_set<UMLAttribute *> UMLInterface::getOperations() const {
    std::unordered_set<UMLAttribute *> operations;
    operations.insert(operations_.begin(), operations_.end());
    // get operations from base classes
    for (auto cls : parentClasses_) {
        for (auto a : cls->getOperations()) {
            if (dynamic_cast<UMLOperation *>(a) != nullptr)
                operations.insert(a);
        }
    }
    return operations;
}
