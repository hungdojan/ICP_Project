//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_GCLASSDIAGRAM_H
#define DIAGRAMEDITOR_GCLASSDIAGRAM_H

#include "GClassifier.h"
#include "GraphicsScene.h"
#include "ClassDiagram.h"
#include "GClassSettings.h"


class GClassDiagram: public QObject{
Q_OBJECT
public:
    GClassDiagram() =delete;
    GClassDiagram(GraphicsScene *scene, ClassDiagram *model);
    void setModel(ClassDiagram *model);
    const ClassDiagram *model() const;
    void scene(GraphicsScene *scene);
    const GraphicsScene *scene() const;
//    ~GClassDiagram();
private slots:
    void addClassifier();
    void onGClassifierSelectionChanged();
private:
    ClassDiagram *classDiagramModel;
    GraphicsScene *scene_;
    std::vector<GClassifier*> gClassifiers;
    GClassSettings *gClassSettings;
};

#endif //DIAGRAMEDITOR_GCLASSDIAGRAM_H
