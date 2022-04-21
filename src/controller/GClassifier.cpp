//
// Created by darbix on 21.4.22.
//

#include <QBrush>
#include "GClassifier.h"
#include "mainwindow.h"
#include <GraphicsScene.h>
#include "UMLClassifier.h"
#include <QDebug>

GClassifier::GClassifier(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent) :
        QGraphicsRectItem(x, y, width, height, parent), QObject(){

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setBrush(QBrush(Qt::white));

    umlClassifier = new UMLClassifier("todo_name", false);

    title = new QGraphicsTextItem(QString::fromStdString(umlClassifier->name()), this);
    title->setPos(x, y);
}
GClassifier::~GClassifier(){
}

void GClassifier::contentSaved(){
    title->setPlainText(QString::fromStdString(umlClassifier->name()));
}

QVariant GClassifier::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemSelectedChange){
        if (value == true) {
            setBrush(QBrush(Qt::gray));
        }
        else {
            setBrush(QBrush(Qt::lightGray));
        }
        emit gClassifierSelectionChanged();
    }
    if (change == ItemPositionChange && scene()) {
        emit gClassifierPositionChanged();
    }
    return QGraphicsItem::itemChange(change, value);
}
