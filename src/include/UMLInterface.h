#ifndef DIAGRAMEDITOR_UMLINTERFACE_H
#define DIAGRAMEDITOR_UMLINTERFACE_H
#include <vector>
#include <unordered_set>
#include "UMLClassifier.h"
#include "UMLOperation.h"

class UMLInterface : public UMLClassifier {
protected:
    std::vector<UMLOperation *> operations_;
    std::unordered_set<UMLInterface *> parentInterfaces_;
    using UMLClassifier::isAbstract;
public:
    UMLInterface() =delete;
    explicit UMLInterface(const std::string &name);
    bool addOperation(UMLOperation *operation);
    UMLOperation *getOperation(const std::string& name) const;
    const bool containsOperation(const UMLOperation *operation) const;
    const std::vector<UMLOperation *> &operations() const;
    bool removeOperation(UMLOperation *operation);
    bool addParentClassifier(UMLInterface *interface);
    const std::unordered_set<UMLInterface *> &parentInterfaces() const;
    const bool containsParentInterface(UMLInterface *interface) const;
    bool removeParentInterface(UMLInterface *interface);

    // TODO: destructor
    ~UMLInterface();
};

#endif //DIAGRAMEDITOR_UMLINTERFACE_H