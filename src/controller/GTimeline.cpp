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
#define HEAD_W 120
#define HEAD_H 50
#define FRAME_W 13

GTimeline::GTimeline(UMLClass *cls, QString name, GraphicsScene *scene, qreal x, qreal y, int indexes): scene{scene},
    dashHeight{1.0 * indexes * MSG_GAP}, cls{cls}, name{name}, indexes{indexes}{

    head = createHead(x, y);

    dashedLine = createDashedLine();

    for(int i = 0; i < indexes; i++)
        frameCreate(i);

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(onFrameSelected()));
}

qreal GTimeline::getX(){
    return dashedLine->sceneBoundingRect().x();
}

GObject *GTimeline::createHead(qreal x, qreal y){
    auto head = new GObject(QString::fromStdString(cls->name()), name, x, y, HEAD_W, HEAD_H);
    connect(head, SIGNAL(gObjectDeleted()), this, SLOT(onGObjectDeleted()));
    connect(this, SIGNAL(gObjectResize()), head, SLOT(onGObjectResize()));
    scene->addItem(head);
    return head;
}

QGraphicsLineItem *GTimeline::createDashedLine(){
    auto dashedLine = new QGraphicsLineItem(head);
    dashedLine->setLine(head->boundingRect().center().x(), head->boundingRect().center().y(),
                        head->boundingRect().center().x(), head->boundingRect().center().y()+dashHeight);
    dashedLine->setZValue(-1);
    dashedLine->setFlag(QGraphicsItem::ItemNegativeZStacksBehindParent);

    QPen pen;
    pen.setWidth(DASH_WIDTH);
    pen.setDashPattern({ 5, 3});
    dashedLine->setPen(pen);
    return dashedLine;
}

void GTimeline::onGObjectDeleted(){
    // Delegate to GSequenceDiagram vectors
    emit gTimelineDeleted();
}

GMessage *GTimeline::addMsg(QString name, GTimeline *target, enum GMessage::direction dir, int index){
    auto msg = new GMessage(scene, name, dir, this, target, index);

    return msg;
}

void GTimeline::changePos(qreal x, qreal y){
    // Remember old frame selections before move
    for(auto f: frames) {
        oldSelected.push_back(f->brush() == QBrush(QColor(SELECTED_COLOR))); // isSelected bool
    }
    frames.clear();
    // delete dashedLine;
    delete head;
    head = createHead(x, y);
    dashedLine = createDashedLine();
    for(int i = 0; i < indexes; i++) {
        frameCreate(i, oldSelected[i]);
    }
}

void GTimeline::frameCreate(int index, bool isSelected){
    auto msgRect = new QGraphicsRectItem(getX() - FRAME_W / 2.0 + 3 / 2.0, MSG_GAP * index, FRAME_W, MSG_GAP, dashedLine);
    frames.push_back(msgRect);
    msgRect->setFlag(QGraphicsItem::ItemIsSelectable);
    msgRect->setPen(Qt::NoPen);
    msgRect->setBrush(QBrush(Qt::transparent));

    if(isSelected)
        msgRect->setBrush(QBrush(QColor(SELECTED_COLOR)));
}

void GTimeline::onFrameSelected() {
    if (!frames.empty()){
        for (auto rect: frames) {
            if(scene->selectedItems().contains(rect)){
                if(rect->brush() == QBrush(QColor(SELECTED_COLOR))) {
                    rect->setBrush(QBrush(Qt::transparent));
                }
                else
                    rect->setBrush(QBrush(QColor(SELECTED_COLOR)));
            }

//            if(rect->isSelected() && rect->brush() == QBrush(QColor(SELECTED_COLOR))) {
//                rect->setBrush(QBrush(QBrush(Qt::transparent)));
//                qDebug() << rect;
//            }
//            else if(rect->isSelected())
//                rect->setBrush(QBrush(QColor(SELECTED_COLOR)));
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
    head->gClassName->setPlainText(QString::fromStdString("("+cls->name())+")");
    emit gObjectResize();
}