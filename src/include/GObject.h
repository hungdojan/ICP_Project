/**
 * @brief Declaration of GObject class
 * Sequence diagram instance object of the UMLClass
 * Timeline head rectangle
 *
 * @file GObject.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_GOBJECT_H
#define DIAGRAMEDITOR_GOBJECT_H

#include <QObject>
#include <QGraphicsRectItem>
#include "GText.h"
#include "UMLObject.h"

/**
 * Graphic representation of the sequence diagram class object
 */
class GObject: public QObject, public QGraphicsRectItem {
Q_OBJECT
public:
//    GObject(QString className, QString name, qreal x, qreal y, qreal w, qreal h);
    /**
     * @brief Constructor of the rectangle object
     * @param model UML model
     * @param x X scene coordinate
     * @param y Y scene coordinate
     * @param w Width
     * @param h Height
     */
    GObject(UMLObject *model, qreal x, qreal y, qreal w, qreal h);
    GObject(QString className, QString name, qreal x, qreal y, qreal w, qreal h);
    /** Instance name */
    GText *gName;
    /** Class name */
    GText *gClassName;

signals:
    /**
     * @brief Signal to center text when class name enlarges
     */
    void centerText();
    /**
     * @brief Signal to inform diagram about GObject deletion
     */
    void gObjectDeleted();

private:
    /** UML model */
    UMLObject *model;
    /** Base itemChange implementation */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    /** Base keyReleaseEvent to choose an object to delete */
    void keyReleaseEvent(QKeyEvent *event);
    qreal x;
    qreal y;
    qreal height;
    qreal width;
private slots:
    /**
     * Slot to receive a resize signal to enlarge a GObject
     */
    void onGObjectResize();
};

#endif //DIAGRAMEDITOR_GOBJECT_H
