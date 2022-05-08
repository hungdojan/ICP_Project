/**
 * @brief Declaration of GTimeline class
 * Sequence diagram instance timeline for messages
 *
 * @file GTimeline.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_GTIMELINE_H
#define DIAGRAMEDITOR_GTIMELINE_H

#include <QGraphicsLineItem>
#include "GraphicsScene.h"
#include "GObject.h"
#include "GMessage.h"
#include "UMLClass.h"

/**
 * @brief TimeLine for the messages
 */
class GTimeline: public QObject{
Q_OBJECT
public:
    /**
     * @brief Constructor to create a blank object
     * @param cls Base class
     * @param name Instance name
     * @param scene GraphicsScene to place in
     * @param x X position
     * @param y Y position
     * @param indexes Max number of indexes (messages)
     */
    GTimeline(UMLClass *cls, QString name, GraphicsScene *scene, qreal x, qreal y, int indexes);
    /**
     * @brief Constructor to load from UML data
     * @param model Object model
     * @param scene Graphic scene in the view
     * @param x X position
     * @param y Y position
     * @param indexes Max number of messages
     */
    GTimeline(UMLObject *model, GraphicsScene *scene, qreal x, qreal y, int indexes);
    ~GTimeline();
    /**
     * @brief Get the X coordinate of the timeline
     * @return
     */
    qreal getX();
    /**
     * @brief Create a life frame for a message index
     * @param index
     * @param isSelected
     */
    void frameCreate(int index, bool isSelected = false);
    /**
     * @brief Get an instance name
     * @return QString name
     */
    QString getName();
    /** Class model */
    UMLClass *cls;
    /**
     * @brief Change position when it needs to move
     * @param x New X position
     * @param y New Y position
     */
    void changePos(qreal x, qreal y);
    /**
     * @brief Get a data model of the object
     * @return UMLObject model
     */
    UMLObject *model();

signals:
    /**
     * @brief Signal sequence diagram when this timeline should be deleted
     */
    void gTimelineDeleted();
    /**
     * Signal to inform GObject to resize when a name is changed
     */
    void gObjectResize();

private:
    /** UML model for the head object instance */
    UMLObject *model_;
    /** Graphic scene pointer */
    GraphicsScene *scene;
    /** Dashed line as a timeline parent for the messages */
    QGraphicsLineItem *dashedLine;
    /** Main top GObject as an instance of the UMLClass */
    GObject *head;
    /** Vector of all frames on the dashedLine */
    std::vector<QGraphicsRectItem*> frames;
    /** Height of the timeline line */
    qreal dashHeight;
    /** Instance name */
    QString name;
    /**
     * @brief Create a GObject to represent a class instance
     * @param x X position
     * @param y Y position
     * @return New GObject
     */
    GObject *createHead(qreal x, qreal y);
    /**
     * @bried Create a dashed line as a lifetime line for messages
     * @return New line
     */
    QGraphicsLineItem *createDashedLine();
    /** Max number of indexes (messages) in the timeline */
    int indexes;

private slots:
    /**
     * @brief Slot to catch a selection change situation on life cycle rectangles (frames)
     */
    void onFrameSelected();
    /**
     * @brief Slot called when a head is deleted
     */
    void onGObjectDeleted();
    /**
     * @brief Slot for the UML class content changes
     */
    void onClassContentUpdated();
};

#endif //DIAGRAMEDITOR_GTIMELINE_H
