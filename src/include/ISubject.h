//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_ISUBJECT_H
#define DIAGRAMEDITOR_ISUBJECT_H
#include "IObserver.h"

class ISubject {
public:
    virtual ~ISubject() =default;
    virtual void attach(IObserver *observer) =0;
    virtual void detach(IObserver *observer) =0;
    virtual void notify(const std::string &msg) =0;
};

#endif //DIAGRAMEDITOR_ISUBJECT_H
