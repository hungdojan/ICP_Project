//
// Created by darbix on 26.4.22.
//

#ifndef DIAGRAMEDITOR_GTIMELINE_H
#define DIAGRAMEDITOR_GTIMELINE_H

#include <QGraphicsLineItem>
#include "GraphicsScene.h"
#include "GObject.h"
#include "GMessage.h"
#include "UMLClass.h"

class GTimeline: public QObject{
Q_OBJECT
public:
    enum type{SEND, RECEIVE, SELF};
    GTimeline(UMLClass *cls, QString name, GraphicsScene *scene, qreal x, qreal y, int indexes);
    ~GTimeline();
    qreal getX();
    GMessage *addMsg(QString name, GTimeline *target, enum GMessage::direction dir, int index);
    void frameCreate(int index);
    QString getName();
    UMLClass *cls;
signals:
    void gTimelineDeleted();
    void gObjectResize();

private:
    GraphicsScene *scene;
    QGraphicsLineItem *dashedLine;
    GObject *head;
    std::vector<QGraphicsRectItem*> frames;
    bool infFrame;
    qreal dashHeight;
private slots:
    void onFrameSelected();
    void onGObjectDeleted();
    void onClassContentUpdated();
};

#endif //DIAGRAMEDITOR_GTIMELINE_H
