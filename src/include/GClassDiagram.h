//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_GCLASSDIAGRAM_H
#define DIAGRAMEDITOR_GCLASSDIAGRAM_H

#include "GClassifier.h"
#include "GraphicsScene.h"


class GClassDiagram: public QObject{
Q_OBJECT
public:
    GClassDiagram(GraphicsScene *scene);
//    ~GClassDiagram();
private slots:
    void addClassifier();
private:
    GraphicsScene *scene;
};

#endif //DIAGRAMEDITOR_GCLASSDIAGRAM_H
