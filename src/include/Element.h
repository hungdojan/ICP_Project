//
// Created by rebulien on 4/2/22.
//

#ifndef DIAGRAMEDITOR_ELEMENT_H
#define DIAGRAMEDITOR_ELEMENT_H
#include <string>
#include <utility>

class Element {
protected:
    /** Element's name */
    std::string name_;
public:
    Element() =delete;
    /**
     * @brief Class constructor.
     * @param name Element's name.
     */
    explicit Element(std::string name) : name_{std::move(name)} { };

    /**
     * @brief Returns read-only reference to element's name.
     * @return Read-only reference of element's name.
     */
    const std::string &name() const;
    virtual ~Element() =default;

protected:
    /**
     * @brief Updates element's name.
     * @param newName New element's name.
     */
    virtual void setName(const std::string &newName);
};


#endif //DIAGRAMEDITOR_ELEMENT_H
