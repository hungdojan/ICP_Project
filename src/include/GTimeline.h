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
    GTimeline(UMLObject *model, GraphicsScene *scene, qreal x, qreal y, int indexes);
    ~GTimeline();
    qreal getX();
    GMessage *addMsg(QString name, GTimeline *target, enum GMessage::direction dir, QString type, int index);
    void frameCreate(int index, bool isSelected = false);
    QString getName();
    UMLClass *cls;
    void changePos(qreal x, qreal y);
    UMLObject *model();
signals:
    void gTimelineDeleted();
    void gObjectResize();

private:
    UMLObject *model_;
    GraphicsScene *scene;
    QGraphicsLineItem *dashedLine;
    GObject *head;
    std::vector<QGraphicsRectItem*> frames;
    qreal dashHeight;
    QString name;
    GObject *createHead(qreal x, qreal y);
    QGraphicsLineItem *createDashedLine();
    int indexes;
    std::vector<bool> oldSelected;
private slots:
    void onFrameSelected();
    void onGObjectDeleted();
    void onClassContentUpdated();
};

#endif //DIAGRAMEDITOR_GTIMELINE_H
