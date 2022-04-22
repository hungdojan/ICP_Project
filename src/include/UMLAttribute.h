//
// Created by rebulien on 4/19/22.
//

#ifndef DIAGRAMEDITOR_UMLATTRIBUTE_H
#define DIAGRAMEDITOR_UMLATTRIBUTE_H
#include "Element.h"
#include "UMLClassifier.h"

class UMLAttribute : public Element {
    // TODO:
protected:
    char visibility_{};
    UMLClassifier *type_;
public:
    UMLAttribute() =delete;
    // Class constructor
    explicit UMLAttribute(const std::string &name, UMLClassifier *classifier) : Element(name), type_{classifier} { }
    // Updates attributes name
    void setName(const std::string &name) override;
    // Returns attribute's visibility
    char &visibility();
    // Returns attribute's type
    const UMLClassifier *type() const;
    // Sets attribute's type
    UMLClassifier *setType(UMLClassifier *classifier);

    bool operator==(UMLAttribute &attr) const;
    virtual explicit operator std::string() const;
    friend std::ostream& operator<<(std::ostream &strm, const UMLAttribute &attr);
    ~UMLAttribute();
};


#endif //DIAGRAMEDITOR_UMLATTRIBUTE_H
