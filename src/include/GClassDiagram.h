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
    void addClassifierInterface();
    void onGClassifierSelectionChanged();
    void onGClassifierContentChanged();
private:
    ClassDiagram *classDiagramModel;
    GraphicsScene *scene_;
    std::vector<GClassifier*> gClassifiers;
    std::vector<GRelation*> gRelations;
    GClassSettings *gClassSettings;
    int name_index{0};
    void addGClassifier(GClassifier *g);
};

#endif //DIAGRAMEDITOR_GCLASSDIAGRAM_H
