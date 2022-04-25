//
// Created by rebulien on 4/20/22.
//

#ifndef DIAGRAMEDITOR_UMLOPERATION_H
#define DIAGRAMEDITOR_UMLOPERATION_H
#include <vector>
#include "Element.h"
#include "UMLAttribute.h"
#include "IObserver.h"

class UMLOperation : public UMLAttribute {
protected:
    std::vector<UMLAttribute*> parameters_;
public:
    UMLOperation() =delete;
    // Class constructor
    UMLOperation(const std::string &name, UMLClassifier *type) : UMLAttribute(name, type) { }
    // Class constructor
    UMLOperation(const std::string &name, UMLClassifier *type, const std::vector<UMLAttribute*> &parameters);
    // Returns collection of parameters of this operation
    const std::vector<UMLAttribute*> &parameters() const;
    // Adds new parameter to the operation
    bool addParameter(UMLAttribute *parameter);
    // Adds new parameter to the operation
    UMLAttribute *addOrReplaceParameter(UMLAttribute *parameter);
    // Returns parameter with given name
    UMLAttribute *getParameterByName(const std::string &name) const;
    // Removes parameter with given name
    bool removeParameter(const std::string &name);
    // Removes instance of parameter from the operation
    bool removeParameter(UMLAttribute *parameter);
    // Clear collection of operation parameters
    void clearParameters(std::vector<UMLAttribute *> *vectorContent);

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createJsonObject(QJsonObject &object) override;

    // bool operator==(UMLOperation &operation) const;
    explicit operator std::string() const final;
    friend std::ostream &operator<<(std::ostream &strm, const UMLOperation &operation);

    ~UMLOperation() override;
};


#endif //DIAGRAMEDITOR_UMLOPERATION_H
