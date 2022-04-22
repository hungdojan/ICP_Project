//
// Created by darbix on 21.4.22.
//

#include "GText.h"
#include "GClassifier.h"
#include <QVariant>
#include <QDebug>

GText::GText(const QString &text, qreal x, qreal y, QGraphicsItem *parent): QGraphicsTextItem(text, parent){
    this->parent = parent;
    setPos(x, y);
}

void GText::centerText() {
    // Horizontal center in a parent
    setPos(parent->boundingRect().x() + (parent->boundingRect().center().x() - sceneBoundingRect().width()/2.0), y());
}

void GText::onTextChanged(){
    // Text width increases all parent rectangles widths
    QGraphicsItem *parentItem = parent;
    QGraphicsRectItem *rect;

    while(rect = dynamic_cast<QGraphicsRectItem*>(parentItem)){
        if(sceneBoundingRect().width() > rect->sceneBoundingRect().width()){
            rect->setRect(boundingRect().x(), boundingRect().y(), sceneBoundingRect().width(), rect->sceneBoundingRect().height());
        }
        parentItem = rect->parentItem();
    }
}
