#include "GRelation.h"
#include "GClassifier.h"
#include "GraphicsScene.h"
#include <QDebug>
#include <QPen>

GRelation::GRelation(QGraphicsItem *parent, QGraphicsItem *target): QObject(), QGraphicsLineItem(NULL) {
    this->parent = parent;
    this->target = target;
    setZValue(-1);
    QPointF p1 = parent->sceneBoundingRect().center();
    QPointF p2 = target->sceneBoundingRect().center();
    setLine(QLineF(p1, p2));
    auto pen = QPen();
    pen.setWidth(2);
    setPen(pen);

    connect(((GClassifier *)parent), SIGNAL(gClassifierPositionChanged()), (GRelation*)this, SLOT(updatePos()));
    connect(((GClassifier *)target), SIGNAL(gClassifierPositionChanged()), (GRelation*)this, SLOT(updatePos()));

}
void GRelation::updatePos() {
    QPointF p1 = this->parent->sceneBoundingRect().center();
    QPointF p2 = this->target->sceneBoundingRect().center();
    setLine(QLineF(p1, p2));
}