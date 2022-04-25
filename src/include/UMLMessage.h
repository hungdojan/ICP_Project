#ifndef DIAGRAMEDITOR_UMLMESSAGE
#define DIAGRAMEDITOR_UMLMESSAGE
#include "Element.h"
#include "UMLOperation.h"
#include "UMLObject.h"

class UMLMessage : public Element {
    UMLObject src_;
    UMLObject dst_;
    UMLOperation *baseOperation_;
public:
    UMLMessage() =delete;
    explicit UMLMessage(UMLOperation *baseOperation, UMLObject &src, UMLObject &dst)
            : Element{""}, src_{src}, dst_{dst}, baseOperation_{baseOperation} { }

    const UMLObject &dst() const;
    const UMLObject &src() const;
    const UMLOperation *baseOperation() const;
    bool setOperation(UMLOperation *operation);
    bool setOperation(const std::string &operationName);

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createJsonObject(QJsonObject &object) override { /* TODO: */ };
};
#endif //DIAGRAMEDITOR_UMLMESSAGE