/**
 * @brief Graphic representation of UML relation in a class diagram
 *
 * @file GRelation.cpp
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "GRelation.h"
#include "GClassifier.h"
#include "GraphicsScene.h"
#include <QDebug>
#include <QPen>
#include <QtMath>

GRelation::GRelation(QGraphicsItem *parent, QGraphicsItem *target, QString srcMsg, QString dstMsg):
    QObject(), QGraphicsLineItem{NULL}{

    this->parent = parent;
    this->target = target;
    setZValue(-1);
    p1 = parent->sceneBoundingRect().center();
    p2 = target->sceneBoundingRect().center();
    setLine(QLineF(p1, p2));
    auto pen = QPen();
    pen.setWidth(2);
    setPen(pen);
    parent->scene()->addItem(this);

    gSrc = new QGraphicsTextItem(srcMsg, this);
    gDst = new QGraphicsTextItem(dstMsg, this);
    centerText(gSrc, parent);
    centerText(gDst, target);

    connect(((GClassifier *)parent), SIGNAL(gClassifierPositionChanged()), (GRelation*)this, SLOT(updatePos()));
    connect(((GClassifier *)target), SIGNAL(gClassifierPositionChanged()), (GRelation*)this, SLOT(updatePos()));
}

void GRelation::updateContent(QString srcMsg, QString dstMsg){
    this->gSrc->setPlainText(srcMsg);
    this->gDst->setPlainText(dstMsg);
    centerText(gSrc, parent);
    centerText(gDst, target);
}

void GRelation::updatePos() {
    p1 = parent->sceneBoundingRect().center();
    p2 = target->sceneBoundingRect().center();

    centerText(gSrc, parent);
    centerText(gDst, target);

    setLine(QLineF(p1, p2));
}

void GRelation::centerText(QGraphicsTextItem *text, QGraphicsItem *rectangle){
    text->setPos(getTextPoint(rectangle));

    QString symbol = text->toPlainText();

    QFont f;
    f.setPointSize(13); // default size

    // Adjustment of special symbols
    if(symbol == "◆" || symbol == "◇" || symbol == "◁") {
        f.setPointSize(30);

        QPointF u = QPointF(p2 - p1);
        double angle = atan(u.y()/u.x())* 180 / M_PI;

        // Generalization symbol has to be inverted at some cases
        if(symbol == "◁" && (text == this->gSrc && p2.x() < p1.x() || text == this->gDst && p2.x() > p1.x()))
            angle += 180;

        text->setRotation(angle);
    }
    else
        text->setRotation(0);

    text->setFont(f);

    // Center of the text rectangle will be on the line
    double pointOnLineX = 2*text->x() - text->sceneBoundingRect().center().x();
    double pointOnLineY = 2*text->y() - text->sceneBoundingRect().center().y();

    text->setPos(pointOnLineX, pointOnLineY);

    // White background so the text can be seen
    text->setHtml("<div style='background-color:#FFFFFF;'>" + symbol + "</div>");
}

QPointF GRelation::getTextPoint(QGraphicsItem *rectangle){
    // Rectangle circle
    QPointF p = rectangle->sceneBoundingRect().center();

    // 1/2 of class rectangle diagonal is a circle from the center to corners
    // 3/4 is a bit larger, so the text is always visible
    double diagonal = sqrt(pow(rectangle->sceneBoundingRect().width(),2)+pow(rectangle->sceneBoundingRect().height(),2)) * 3/4;

    double cx = sceneBoundingRect().center().x();
    double cy = sceneBoundingRect().center().y();
    double nx = sceneBoundingRect().center().x();
    double ny = sceneBoundingRect().center().y();

    const int maxSteps = 20;
    const int partsOfMovement = 15;
    int steps = 0;
    while(sqrt(pow(p.x()-nx,2)+pow(p.y()-ny,2)) > diagonal && steps < maxSteps){
        nx += (p.x()-cx)/partsOfMovement;
        ny += (p.y()-cy)/partsOfMovement;
        steps++;
    }

    return QPointF(nx, ny);
}

QGraphicsItem *GRelation::getSrc(){
    return parent;
}

QGraphicsItem *GRelation::getDst(){
    return target;
}