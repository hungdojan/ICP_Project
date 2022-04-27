//
// Created by darbix on 26.4.22.
//

#include "GTimeline.h"
#include <QGraphicsLineItem>
#include "GObject.h"
#include "UMLClass.h"
#include <QDebug>

#define DASH_WIDTH 3
#define MSG_GAP 100
#define SELECTED_COLOR "#9999A9"


GTimeline::GTimeline(UMLClass *cls, QString name, GraphicsScene *scene, qreal x, qreal y, int indexes): scene{scene}, infFrame{false},
    dashHeight{1.0 * indexes * MSG_GAP}, cls{cls}{

    head = new GObject(QString::fromStdString(cls->name()), name, x, y, 120, 50);
    connect(head, SIGNAL(gObjectDeleted()), this, SLOT(onGObjectDeleted()));
    connect(this, SIGNAL(gObjectResize()), head, SLOT(onGObjectResize()));
    scene->addItem(head);

    dashedLine = new QGraphicsLineItem(head);
    dashedLine->setLine(head->boundingRect().center().x(), head->boundingRect().center().y(),
                        head->boundingRect().center().x(), head->boundingRect().center().y()+dashHeight);
    dashedLine->setZValue(-1);
    dashedLine->setFlag(QGraphicsItem::ItemNegativeZStacksBehindParent);

    QPen pen;
    pen.setWidth(DASH_WIDTH);
    pen.setDashPattern({ 5, 3});
    dashedLine->setPen(pen);

    for(int i = 0; i < indexes; i++){
        frameCreate(i);
    }
}

qreal GTimeline::getX(){
    return dashedLine->sceneBoundingRect().x();
}

void GTimeline::onGObjectDeleted(){
    // Delegate to GSequenceDiagram vectors
    emit gTimelineDeleted();
}

GMessage *GTimeline::addMsg(QString name, GTimeline *target, enum GMessage::direction dir, int index){
    auto msg = new GMessage(scene, name, dir, this, target, index);

    return msg;
}


void GTimeline::frameCreate(int index){
    const qreal rectWidth = 13;
    auto msgRect = new QGraphicsRectItem(this->getX() - rectWidth / 2.0 + 3 / 2.0, MSG_GAP * index, rectWidth,MSG_GAP, dashedLine);
    msgRect->setBrush(QBrush(QColor("#FFFFFF")));
    frames.push_back(msgRect);
    msgRect->setFlag(QGraphicsItem::ItemIsSelectable);
    msgRect->setPen(Qt::NoPen);
    msgRect->setBrush(QBrush(Qt::transparent));
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(onFrameSelected()));
}

void GTimeline::onFrameSelected() {
    if (!frames.empty()){
        for (auto rect: frames) {
            if(rect->isSelected() && rect->brush() == QBrush(QColor(SELECTED_COLOR)))
                rect->setBrush(QBrush(QBrush(Qt::transparent)));
            else if(rect->isSelected())
                rect->setBrush(QBrush(QColor(SELECTED_COLOR)));
        }
    }
}

GTimeline::~GTimeline(){
    frames.clear();
    delete head;
    //todo
}

QString GTimeline::getName(){
    return head->gName->toPlainText();
}

void GTimeline::onClassContentUpdated(){
    //todo gtimeline resize rect??";
    head->gClassName->setPlainText(QString::fromStdString("("+cls->name())+")");
    emit gObjectResize();
}