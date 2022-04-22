//
// Created by rebulien on 4/20/22.
//

#ifndef DIAGRAMEDITOR_UMLOPERATION_H
#define DIAGRAMEDITOR_UMLOPERATION_H
#include <vector>
#include "Element.h"
#include "UMLAttribute.h"

class UMLOperation : public UMLAttribute {
    std::vector<UMLAttribute> parameters_;
public:
    UMLOperation() =delete;
    // Class constructor
    UMLOperation(const std::string &name, UMLClassifier *type) : UMLAttribute(name, type) { }
    // Class constructor
    UMLOperation(const std::string &name, UMLClassifier *type, std::vector<UMLAttribute>parameters);
    // Returns collection of parameters of this operation
    const std::vector<UMLAttribute> &parameters() const;
    // Adds new parameter to the operation
    bool addParameter(const UMLAttribute &parameter);
    // Adds new parameter to the operation
    bool addOrReplaceParameter(const UMLAttribute &parameter, UMLAttribute *oldInstance);
    // Returns parameter with given name
    bool getParameterByName(const std::string &name, UMLAttribute *found);
    // Removes parameter with given name
    bool removeParameter(const std::string &name, UMLAttribute *removed);
    // Removes instance of parameter from the operation
    bool removeParameter(const UMLAttribute &parameter);
    // Clear collection of operation parameters
    void clearParameters(std::vector<UMLAttribute> *vectorContent);

    // bool operator==(UMLOperation &operation) const;
    explicit operator std::string() const final;
    friend std::ostream &operator<<(std::ostream &strm, const UMLOperation &operation);

    ~UMLOperation();
};


#endif //DIAGRAMEDITOR_UMLOPERATION_H
