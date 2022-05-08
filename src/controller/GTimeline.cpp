/**
 * @brief Sequence diagram instance timeline for messages
 *
 * @file GTimeline.cpp
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "GTimeline.h"
#include <QGraphicsLineItem>
#include "GObject.h"
#include "UMLClass.h"
#include <QDebug>

#define DASH_WIDTH 3
#define MSG_GAP 80
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
GTimeline::GTimeline(UMLObject *model, GraphicsScene *scene, qreal x, qreal y, int indexes) :
        QObject{}, model_{model}, scene{scene}, dashHeight{1.0 * indexes * MSG_GAP}, indexes{indexes}{
    head = createHead(x, y);

    dashedLine = createDashedLine();

    for(int i = 0; i < indexes; i++)
        frameCreate(i);

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(onFrameSelected()));
}

qreal GTimeline::getX(){
    return dashedLine->sceneBoundingRect().x();
}

UMLObject *GTimeline::model() { return model_; }

GObject *GTimeline::createHead(qreal x, qreal y){
    auto head = new GObject(model_, x, y, HEAD_W, HEAD_H);
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

void GTimeline::changePos(qreal x, qreal y){
//    // Remember old frame selections before move
    std::vector<bool>oldSelected;
    for(auto f: frames) {
        oldSelected.push_back(f->brush().color() == QColor(SELECTED_COLOR)); // isSelected bool
    }

    delete head; // cascade delete dashedLine and frames with parent
    head = createHead(x, y);
    dashedLine = createDashedLine();
    frames.clear();

    for(int i = 0; i < indexes; i++) {
        frameCreate(i, oldSelected[i]);
    }
    oldSelected.clear();
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
        }
    }
}

GTimeline::~GTimeline(){
    delete head;
}

QString GTimeline::getName(){
    return head->gName->toPlainText();
}

void GTimeline::onClassContentUpdated(){
    head->gClassName->setPlainText(QString::fromStdString("("+model_->model()->name())+")");
    emit gObjectResize();
}