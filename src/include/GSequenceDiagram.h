//
// Created by darbix on 26.4.22.
//

#ifndef DIAGRAMEDITOR_GSEQUENCEDIAGRAM_H
#define DIAGRAMEDITOR_GSEQUENCEDIAGRAM_H

//#include <QObject>
#include "GraphicsScene.h"
#include "ClassDiagram.h"
#include "GTimeline.h"
#include <QFrame>

class GSequenceDiagram: public QObject{
Q_OBJECT
public:
//    ~GSequenceDiagram();
    GSequenceDiagram(GraphicsScene *scene, std::string name, ClassDiagram *classDiagram, QFrame *settings);
signals:
    void classContentUpdated();
private:
    ClassDiagram *classDiagram;
    SequenceDiagram *sequanceDiagram;
    QFrame *settings;
    void addSettings();
    std::vector<GTimeline*> gTimelines;
    std::vector<GMessage*> gMessages;
    int index;
    GraphicsScene *scene;
private slots:
    void onClassDiagramUpdated();
    void onAddPressed();
//    void onDeletePressed();
    void onFuncUpdate();
    void onGTimelineDeleted();
    void onSaveMsg();
};

#endif //DIAGRAMEDITOR_GSEQUENCEDIAGRAM_H
