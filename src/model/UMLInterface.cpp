#include "UMLInterface.h"
#include <algorithm>

UMLInterface::UMLInterface(const std::string &name) : UMLClassifier(name) {
    isAbstract_ = true;
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

const bool UMLInterface::containsOperation(const UMLOperation *operation) const {
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

bool UMLInterface::addParentClassifier(UMLInterface *interface) {
    if (interface == nullptr)
        return false;

    auto iter{std::find(parentInterfaces_.begin(), parentInterfaces_.end(), interface)};
    if (iter != parentInterfaces_.end())
        return false;
    parentInterfaces_.insert(interface);
    return true;
}

const std::unordered_set<UMLInterface *> &UMLInterface::parentInterfaces() const {
    return parentInterfaces_;
}

const bool UMLInterface::containsParentInterface(UMLInterface *interface) const {
    if (interface == nullptr)
        return false;
    auto iter{std::find(parentInterfaces_.begin(), parentInterfaces_.end(),interface)};
    return iter == parentInterfaces_.end();
}

bool UMLInterface::removeParentInterface(UMLInterface *interface) {
    if (interface == nullptr)
        return false;
    auto iter{std::find(parentInterfaces_.begin(), parentInterfaces_.end(), interface)};
    if (iter == parentInterfaces_.end())
        return false;
    parentInterfaces_.erase(iter);
    return true;
}