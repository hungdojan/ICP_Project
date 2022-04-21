//
// Created by darbix on 21.4.22.
//

#include <QBrush>
#include "GClassifier.h"
#include "mainwindow.h"
#include <GraphicsScene.h>

GClassifier::GClassifier(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent) :
        QGraphicsRectItem(x, y, width, height, parent), QObject(){

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setBrush(QBrush(Qt::white));
    gClassSettings = nullptr;

    title = new QGraphicsTextItem("blank_title", this);
    title->setPos(x, y);
}
GClassifier::~GClassifier(){
}

void GClassifier::contentSaved(){
//    title->setPlainText(classSettings->title);
}

QVariant GClassifier::itemChange(GraphicsItemChange change, const QVariant &value){
    if (change == QGraphicsItem::ItemSelectedChange){
        if (value == true) {
            setBrush(QBrush(Qt::gray));
            if(gClassSettings == nullptr){
                gClassSettings = (gClassSettings == nullptr)? new GClassSettings(((MainWindow*)scene()->parent())->getCategoryTree()): gClassSettings; //todo
                connect(gClassSettings, SIGNAL(contentSaved()), this, SLOT(contentSaved()));
            }
            else
                gClassSettings->loadCategories();
        }
        else {
            gClassSettings->hideCategories();
            setBrush(QBrush(Qt::lightGray));
        }
    }
    if (change == ItemPositionChange && scene()) {
        emit gClassifierPositionChanged();
    }
    return QGraphicsItem::itemChange(change, value);
}
