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
    enum direction{LTOR, RTOL, LTOL};
    GMessage(GraphicsScene *scene, QString name, enum direction dir, GTimeline *src, GTimeline *dst, QString type, int index);
    ~GMessage();
    GTimeline *src;
    GTimeline *dst;
    std::vector<QGraphicsLineItem*>getItems();
    int index;
    QString getFuncName();
    void warn();
private:
    GraphicsScene *scene;
    QGraphicsLineItem *line;
    QGraphicsLineItem *lineH;
    QGraphicsLineItem *lineBack;
    QString name;
    qreal posY;
    QGraphicsTextItem *text;
    direction dir;
    QString type;

    void addLine(qreal x1, qreal x2, qreal y, enum direction dir);
    void addLtoL(qreal x1, qreal y1, enum direction dir);
    void addArrow(QGraphicsLineItem *line, enum direction dir, qreal x1, qreal x2, qreal y);
    void addText();
private slots:
    void onUpdateMsgPos();
};

#endif //DIAGRAMEDITOR_GMESSAGE_H
