/**
 * @brief Declaration of GRelation class
 * Graphic representation of UML relation in a class diagram
 *
 * @file GRelation.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_GRELATION_H
#define DIAGRAMEDITOR_GRELATION_H

#include <QGraphicsLineItem>

/**
 * Graphic representation of a UMLRelation model
 */
class GRelation : public QObject, public QGraphicsLineItem {
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param parent Source item of the relation
     * @param target Destination target item
     * @param srcMsg Source relation sign
     * @param dstMsg Destination relation sign
     */
    GRelation(QGraphicsItem *parent, QGraphicsItem *target, QString srcMsg, QString dstMsg);
    /**
     * @brief Get a source item
     * @return Source parent of the relation
     */
    QGraphicsItem *getSrc();
    /**
     * @brief Get a destination item
     * @return Target of the relation
     */
    QGraphicsItem *getDst();
    /**
     * @brief Update text contents
     * @param srcMsg New source relation sign
     * @param dstMsg New destination relation sign
     */
    void updateContent(QString srcMsg, QString dstMsg);

private:
    /** Source of the relation */
    QGraphicsItem *parent;
    /** Target of the relation */
    QGraphicsItem *target;
    /** Source text sign */
    QGraphicsTextItem *gSrc;
    /** Destination text sign */
    QGraphicsTextItem *gDst;
    /** relation start point */
    QPointF p1;
    /** relation end point */
    QPointF p2;
    /**
     * @brief Calculate a point where the text should be
     * @param rectangle target rectangle
     * @return point location
     */
    QPointF getTextPoint(QGraphicsItem *rectangle);
    /**
     * @brief Center the text to the line
     * @param text Text object to center
     * @param rectangle Target
     */
    void centerText(QGraphicsTextItem *text, QGraphicsItem *rectangle);

private slots:
    /**
     * @brief Update position when signal change was emitted
     */
    void updatePos();
};

#endif //DIAGRAMEDITOR_GRELATION_H
