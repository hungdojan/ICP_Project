/**
 * @brief Declaration of GMessage class
 * Graphic class for a sequence diagram message
 *
 * @file GMessage.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_GMESSAGE_H
#define DIAGRAMEDITOR_GMESSAGE_H

#include <QGraphicsLineItem>
#include "GraphicsScene.h"
#include "UMLMessage.h"

class GTimeline;
/**
 * @brief Graphic representation of UML sequence diagram message
 */
class GMessage: public QObject{
Q_OBJECT
public:
    /**
     * @brief Possible message directions
     */
    enum direction{
        LTOR, RTOL, LTOL
    };
    /**
     * @brief Constructor to create blank
     * @param scene GraphicScene to be drawn in
     * @param name Text of the message
     * @param dir Direction
     * @param src Source of the message
     * @param dst Destination target
     * @param type Type of the message
     * @param index Global index in the timelines
     */
    GMessage(GraphicsScene *scene, QString name, enum direction dir, GTimeline *src, GTimeline *dst, QString type, int index);
    /**
     * @brief Constructor to load from model
     * @param scene GraphicScene to be drawn in
     * @param name UML model
     * @param dir Direction
     * @param src Source of the message
     * @param dst Destination target
     * @param index Global index in the timelines
     */
    GMessage(GraphicsScene *scene, UMLMessage *model, enum direction dir, GTimeline *src, GTimeline *dst, int index);
    ~GMessage();
    /** Source of the message */
    GTimeline *src;
    /** Destination of the message */
    GTimeline *dst;
    /** UML message model */
    UMLMessage *model();
    /** Get all line part for the destruction*/
    std::vector<QGraphicsLineItem*>getItems();
    int index;
    /**
     * @brief Get the text name of the message
     * @return QString name
     */
    QString getFuncName();
    /**
     * @brief warn with red color, when a function name changed
     * @param warning True if there is an error, False to disable warning
     */
    void warn(bool warning);

private:
    /** GraphicsScene scene plane */
    GraphicsScene *scene;
    /** First message line part */
    QGraphicsLineItem *line;
    /** Hooked vertical line part for the LTOL */
    QGraphicsLineItem *lineH;
    /** Horizontal returning line for the LTOL */
    QGraphicsLineItem *lineBack;
    /** Name of the message */
    QString name;
    /** Y coordinate in the scene */
    qreal posY;
    /** Graphic text object */
    QGraphicsTextItem *text;
    /** Message direction*/
    direction dir;
    /** Message type */
    QString type;
    /** UML model pointer of the message */
    UMLMessage *model_;

    /**
     * @brief Add a single horizontal line as a message
     * @param x1 X coordinate of the start
     * @param x2 X coordinate of the end
     * @param y Y coordinate
     * @param dir Direction
     */
    void addLine(qreal x1, qreal x2, qreal y, enum direction dir);
    /**
     * @brief Add a returning message LTOL to the same timeline
     * @param x1 X of the start
     * @param y1 Y of the start
     * @param dir Direction
     */
    void addLtoL(qreal x1, qreal y1, enum direction dir);
    /**
     * @brief Choose and add an arrow at the end of the message
     * @param line Graphics line to get an arrow
     * @param dir Direction
     * @param x1 X of the message start location
     * @param x2 X of the message end location
     * @param y Y of the location
     */
    void addArrow(QGraphicsLineItem *line, enum direction dir, qreal x1, qreal x2, qreal y);
    /**
     * @brief Add a name text above the message line
     */
    void addText();

private slots:
    /**
     * @brief Slot to get an update of the position due to a timeline move
     */
    void onUpdateMsgPos();
};

#endif //DIAGRAMEDITOR_GMESSAGE_H
