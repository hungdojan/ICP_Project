#ifndef DIAGRAMEDITOR_UMLMESSAGE
#define DIAGRAMEDITOR_UMLMESSAGE
#include "Element.h"
#include "UMLOperation.h"
#include "UMLObject.h"

class UMLMessage : public Element, public IObserver {
    UMLObject *src_;
    UMLObject *dst_;
    UMLOperation *baseOperation_;
public:
    UMLMessage() =delete;
    explicit UMLMessage(UMLOperation *baseOperation, UMLObject *src, UMLObject *dst);
    explicit UMLMessage(const std::string &name, UMLObject *src, UMLObject *dst);

    std::string text() const;
    void setText(const std::string &text);

    const UMLObject *dst() const;
    const UMLObject *src() const;
    const UMLOperation *baseOperation() const;
    bool setOperation(UMLOperation *operation);
    bool setOperation(const std::string &operationName);
    void update(const std::string& msg) override;

    /**
     * @brief Creates JSON representation of element's content.
     * @param object Reference to QJsonObject instance.
     */
    void createJsonObject(QJsonObject &object) override;

    void createJsonObject(QJsonObject &object, int index);
    ~UMLMessage() override;
};
#endif //DIAGRAMEDITOR_UMLMESSAGE