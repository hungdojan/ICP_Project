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
#include <QListWidget>

class GSequenceDiagram: public QObject{
Q_OBJECT
public:
//    ~GSequenceDiagram();
    GSequenceDiagram(GraphicsScene *scene, std::string name, ClassDiagram *classDiagram, QFrame *settings);
signals:
    void classContentUpdated();
    void updateMsgPos();
private:
    ClassDiagram *classDiagram;
    SequenceDiagram *sequanceDiagram;
    QFrame *settings;
    void addSettings();
    std::vector<GTimeline*> gTimelines;
    std::vector<GMessage*> gMessages;
    int index;
    GraphicsScene *scene;
    UMLClassifier *getClassifByInst(QString instName);
    void addMsgItem(QListWidget *qList, GMessage *gMsg);
    QListWidget *msgList;
    void updateAfterMsgDelete();
private slots:
    void onClassDiagramUpdated();
    void onAddPressed();
    void onFuncUpdate();
    void onGTimelineDeleted();
    void onSaveMsg();
    void msgDropped(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    void onMsgDoubleClick(const QModelIndex &ix);
};

#endif //DIAGRAMEDITOR_GSEQUENCEDIAGRAM_H
