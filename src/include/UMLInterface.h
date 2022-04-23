#ifndef DIAGRAMEDITOR_UMLINTERFACE_H
#define DIAGRAMEDITOR_UMLINTERFACE_H
#include <vector>
#include <unordered_set>
#include "UMLClassifier.h"
#include "UMLOperation.h"

class UMLInterface : public UMLClassifier {
protected:
    std::vector<UMLOperation *> operations_;
    using UMLClassifier::isAbstract;
public:
    UMLInterface() =delete;
    explicit UMLInterface(const std::string &name);
    explicit UMLInterface(const std::string &name, const std::vector<UMLOperation *> &operations);
    bool addOperation(UMLOperation *operation);
    void addOperations(const std::vector<UMLOperation *> &operations);
    UMLOperation *getOperation(const std::string& name) const;
    bool containsOperation(const UMLOperation *operation) const;
    const std::vector<UMLOperation *> &operations() const;
    bool removeOperation(UMLOperation *operation);

    // bool addParentClass(UMLClassifier *parentClass) override;
    // bool removeParentClass(UMLClassifier *parentClass) override;
    // UMLClassifier *removeParentClass(const std::string &parentClassName) override;

    bool addRelation(UMLClassifier *dst) override;
    bool removeRelation(UMLRelation *relation) override;
    bool removeRelation(UMLClassifier *dstClass) override;

    // TODO: destructor
    ~UMLInterface() override;
};

#endif //DIAGRAMEDITOR_UMLINTERFACE_H