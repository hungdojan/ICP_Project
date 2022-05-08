/**
 * @brief Graphic text item
 *
 * @file GText.cpp
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

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
    setPos(boundingRect().x() + (parent->boundingRect().center().x() - sceneBoundingRect().width()/2.0), y());
}

