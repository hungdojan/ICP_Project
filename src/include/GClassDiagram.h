//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_GCLASSDIAGRAM_H
#define DIAGRAMEDITOR_GCLASSDIAGRAM_H

#include "GClassifier.h"
#include "GraphicsScene.h"
#include "ClassDiagram.h"
#include "GClassSettings.h"
#include "GSequenceDiagram.h"

class MainWindow;

class GClassDiagram: public QObject{
Q_OBJECT
public:
    GClassDiagram() =delete;
    GClassDiagram(GraphicsScene *scene, ClassDiagram *model, MainWindow *mainWindow);
    void setModel(ClassDiagram *model);
    const ClassDiagram *model() const;
    void scene(GraphicsScene *scene);
    const GraphicsScene *scene() const;
    void loadSequenceDiagram();
signals:
    void updateUndefTypes();
    void classDiagramUpdated();
//    ~GClassDiagram();
private slots:
    void addClassifier();
    void addClassifierInterface();
    void onGClassifierSelectionChanged();
    void onGClassifierContentChanged();
    void onGClassifierDeleted();
private:
    ClassDiagram *classDiagramModel;
    GraphicsScene *scene_;
    std::vector<GClassifier*> gClassifiers;
    std::vector<GRelation*> gRelations;
//    std::vector<GSequenceDiagram *> *gSequenceDiagrams;
    MainWindow *mainWindow;
    GClassSettings *gClassSettings;
    int name_index{0};
    void addGClassifier(GClassifier *g);
    std::string getIndexed(std::string classifierName);
};

#endif //DIAGRAMEDITOR_GCLASSDIAGRAM_H
