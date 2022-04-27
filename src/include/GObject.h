//
// Created by darbix on 26.4.22.
//

#ifndef DIAGRAMEDITOR_GOBJECT_H
#define DIAGRAMEDITOR_GOBJECT_H

#include <QObject>
#include <QGraphicsRectItem>
#include "GText.h"

class GObject: public QObject, public QGraphicsRectItem {
Q_OBJECT
public:
    GObject(QString className, QString name, qreal x, qreal y, qreal w, qreal h);
    GText *gName;
    GText *gClassName;

signals:
    void centerText();
    void gObjectDeleted();
private:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void keyReleaseEvent(QKeyEvent *event);
    qreal x;
    qreal y;
    qreal height;
    qreal width;
private slots:
    void onGObjectResize();
};

#endif //DIAGRAMEDITOR_GOBJECT_H
