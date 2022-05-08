/**
 * @brief Sequence diagram instance object of the UMLClass
 * Timeline head rectangle
 *
 * @file GObject.cpp
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "GObject.h"
#include <QBrush>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QKeyEvent>

#define SELECTED_COLOR "#99DCFF"
#define UNSELECTED_COLOR "#CFCFDD"

GObject::GObject(QString className, QString name, qreal x, qreal y, qreal w, qreal h):
    QObject(), QGraphicsRectItem(), x{x}, y{y}, width{w}, height{h}{

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setFlag(QGraphicsItem::ItemIsFocusable);

    setBrush(QBrush(QColor(UNSELECTED_COLOR)));
    setZValue(1);

    setRect(x, y, w, h);
    gName = new GText(name, x, y, this);
    connect(this, SIGNAL(centerText()), gName, SLOT(centerText()));
    gClassName = new GText("("+className+")", x, y+25, this);
    connect(this, SIGNAL(centerText()), gClassName, SLOT(centerText()));
    emit centerText();
}

GObject::GObject(UMLObject *model, qreal x, qreal y, qreal w, qreal h):
        QObject(), QGraphicsRectItem(), model{model}, x{x}, y{y}, width{w}, height{h}{

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setFlag(QGraphicsItem::ItemIsFocusable);

    setBrush(QBrush(QColor(UNSELECTED_COLOR)));
    setZValue(1);

    // Model must not be nullptr

    setRect(x, y, w, h);
    gName = new GText(QString::fromStdString(model->name()),x, y, this);
    connect(this, SIGNAL(centerText()), gName, SLOT(centerText()));
    gClassName = new GText("("+QString::fromStdString(model->model()->name())+")", x, y+25, this);
    connect(this, SIGNAL(centerText()), gClassName, SLOT(centerText()));
    emit centerText();
}

QVariant GObject::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change == QGraphicsItem::ItemSelectedChange){
        if (value == true)
            setBrush(QBrush(QColor(SELECTED_COLOR)));
        else
            setBrush(QBrush(QColor(UNSELECTED_COLOR)));
    }
    if(change == ItemPositionChange && scene()) {
    }

    return QGraphicsItem::itemChange(change, value);
}

void GObject::keyReleaseEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Delete)
        emit gObjectDeleted();
}

void GObject::onGObjectResize(){
    qreal w = gClassName->boundingRect().width();
    if(w > width){

        setRect(x - (w - width)/2, y, w, height);
        width = w;
    }
    emit centerText();
}