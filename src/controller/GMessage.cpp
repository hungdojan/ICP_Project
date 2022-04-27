//
// Created by darbix on 26.4.22.
//

#include "GMessage.h"
#include <QGraphicsLineItem>
#include "GraphicsScene.h"
#include "GTimeline.h"

#define ARROW_FONT_SIZE 20
#define LINE_WIDTH 2
#define BACK_LINE_LEN 250
#define BACK_LINE_HEIGHT 25

#define MSG_GAP 100

GMessage::GMessage(GraphicsScene *scene, QString name, enum direction dir, GTimeline *src, GTimeline *dst, int index):
        QObject(), scene{scene}, src{src}, dst{dst}, line{nullptr}, lineH{nullptr}, lineBack{nullptr}, index{index}, name{name}{
    if(dir == GMessage::LTOL)
        addLtoL(src->getX(), MSG_GAP*index, dir);
    else
        addLine(src->getX(), dst->getX(), MSG_GAP*index, dir);

    auto text = new QGraphicsTextItem(name, line);
    text->setPos(line->boundingRect().center().x() - text->sceneBoundingRect().width()/2.0, MSG_GAP*index - text->sceneBoundingRect().height());
    text->setHtml("<div style='background-color:#FFFFFF;'>" + text->toPlainText() + "</div>");
}

GMessage::~GMessage(){
    delete line;
    delete lineH;
    delete lineBack;
}

std::vector<QGraphicsLineItem*> GMessage::getItems(){
    std::vector<QGraphicsLineItem*> items;
    if(line) items.push_back(line);
    if(lineH) items.push_back(lineH);
    if(lineBack) items.push_back(lineBack);
    return items;
}

void GMessage::addLtoR(qreal x1, qreal x2, qreal y, enum direction dir){
//    line = new QGraphicsLineItem(x1, y, x2, y);
//    scene->addItem(line);
//
//    addArrow(line, dir, x1, x2, y);
//
//    QPen pen;
//    pen.setWidth(LINE_WIDTH);
//    line->setPen(pen);
}

void GMessage::addLine(qreal x1, qreal x2, qreal y, enum direction dir){
    line = new QGraphicsLineItem(x1, y, x2, y);
    scene->addItem(line);

    addArrow(line, dir, x1, x2, y);

    QPen pen;
    pen.setWidth(LINE_WIDTH);
    if(dir == GMessage::LTOR_RESPONSE || dir == GMessage::RTOL_RESPONSE) {
//        connect(this, SIGNAL(responseClosed()), src, SLOT(onResponseClosed()));
//        emit responseClosed();
        pen.setDashPattern({0, 1, 5, 6});
    }
    line->setPen(pen);
}

void GMessage::addLtoL(qreal x1, qreal y1, enum direction dir){
    line = new QGraphicsLineItem(x1, y1, x1+BACK_LINE_LEN, y1);
    lineH = new QGraphicsLineItem(x1+BACK_LINE_LEN, y1, x1+BACK_LINE_LEN, y1+BACK_LINE_HEIGHT);
    lineBack = new QGraphicsLineItem(x1+BACK_LINE_LEN, y1+BACK_LINE_HEIGHT, x1, y1+BACK_LINE_HEIGHT);

    scene->addItem(line);
    scene->addItem(lineH);
    scene->addItem(lineBack);

    addArrow(lineBack, dir, x1, x1, y1+BACK_LINE_HEIGHT);

    QPen pen;
    pen.setWidth(LINE_WIDTH);
    line->setPen(pen);
    lineH->setPen(pen);
    lineBack->setPen(pen);
}

void GMessage::addArrow(QGraphicsLineItem *line, enum GMessage::direction dir, qreal x1, qreal x2, qreal y){
    QString sign = dir == GMessage::LTOR_RESPONSE || dir == GMessage::LTOR_SEND? "►":"◄";

    auto arrow = new QGraphicsTextItem(sign, line);
    auto font = QFont();
    font.setPointSize(ARROW_FONT_SIZE);
    arrow->setFont(font);

    line->setZValue(1);
//    arrow->setZValue(1);

    const qreal textOffset = 2.5;

    if(dir == GMessage::LTOR_RESPONSE || dir == GMessage::LTOR_SEND)
        arrow->setPos(x2 - arrow->sceneBoundingRect().width() + textOffset*2, y - textOffset - arrow->sceneBoundingRect().height()/2.0);
    else
        arrow->setPos(x2 - textOffset*2, y - textOffset - arrow->sceneBoundingRect().height()/2.0);
}