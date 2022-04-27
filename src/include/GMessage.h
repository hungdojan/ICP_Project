//
// Created by darbix on 26.4.22.
//

#ifndef DIAGRAMEDITOR_GMESSAGE_H
#define DIAGRAMEDITOR_GMESSAGE_H

#include <QGraphicsLineItem>
#include "GraphicsScene.h"
// #include "GTimeline.h"

class GTimeline;
class GMessage: public QObject{
Q_OBJECT
public:
    enum direction{LTOR_SEND, RTOL_SEND, LTOR_RESPONSE, RTOL_RESPONSE, LTOL};
    GMessage(GraphicsScene *scene, QString name, enum direction dir, GTimeline *src, GTimeline *dst, int index);
    ~GMessage();
    GTimeline *src;
    GTimeline *dst;
    std::vector<QGraphicsLineItem*>getItems();
    int index;
signals:
    void responseClosed();
private:
    GraphicsScene *scene;
    QGraphicsLineItem *line;
    QGraphicsLineItem *lineH;
    QGraphicsLineItem *lineBack;
    QString name;

    void addLtoR(qreal x1, qreal x2, qreal y, enum direction dir);
    void addLine(qreal x1, qreal x2, qreal y, enum direction dir);
    void addLtoL(qreal x1, qreal y1, enum direction dir);
    void addArrow(QGraphicsLineItem *line, enum direction dir, qreal x1, qreal x2, qreal y);
};

#endif //DIAGRAMEDITOR_GMESSAGE_H
