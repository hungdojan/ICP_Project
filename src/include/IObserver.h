//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_IOBSERVER_H
#define DIAGRAMEDITOR_IOBSERVER_H
#include <string>

class IObserver {
public:
    ~IObserver() =default;
    virtual void update(const std::string &msg) =0;
};

#endif //DIAGRAMEDITOR_IOBSERVER_H
