#ifndef DIAGRAMEDITOR_GRELATION_H
#define DIAGRAMEDITOR_GRELATION_H

#include <QGraphicsLineItem>

class GRelation : public QObject, public QGraphicsLineItem {
Q_OBJECT
public:
    GRelation(QGraphicsItem *parent, QGraphicsItem *target, QString srcMsg, QString dstMsg);
    QGraphicsItem *getSrc();
    QGraphicsItem *getDst();
    void updateContent(QString srcMsg, QString dstMsg);
private:
    QGraphicsItem *parent;
    QGraphicsItem *target;
    QGraphicsTextItem *gSrc;
    QGraphicsTextItem *gDst;
    QPointF p1;
    QPointF p2;
    QPointF getTextPoint(QGraphicsItem *rectangle);
    void centerText(QGraphicsTextItem *text, QGraphicsItem *rectangle);
private slots:
    void updatePos();
};

#endif //DIAGRAMEDITOR_GRELATION_H
