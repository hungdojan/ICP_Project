//
// Created by rebulien on 4/2/22.
//

#ifndef DIAGRAMEDITOR_ELEMENT_H
#define DIAGRAMEDITOR_ELEMENT_H
#include <string>
#include <utility>

class Element {
protected:
    std::string name_;
public:
    Element() =delete;
    explicit Element(std::string name) : name_{std::move(name)} { };
    const std::string &name() const;

protected:
    virtual void setName(const std::string &newName);
};


#endif //DIAGRAMEDITOR_ELEMENT_H
