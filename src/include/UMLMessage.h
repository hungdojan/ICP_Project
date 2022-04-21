#ifndef DIAGRAMEDITOR_UMLMESSAGE
#define DIAGRAMEDITOR_UMLMESSAGE
#include "Element.h"
#include "UMLOperation.h"

class UMLMessage : public Element {
    UMLOperation *baseOperation_;
public:
    UMLMessage() =delete;
    explicit UMLMessage(const std::string &name, UMLOperation *baseOperation) : Element{name}, baseOperation_{baseOperation} { }


};
#endif //DIAGRAMEDITOR_UMLMESSAGE