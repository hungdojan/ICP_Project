/**
 * @brief Declaration of GSequenceDiagram class
 * Sequence diagram window with graphics for UMLSequenceDiagram
 *
 * @file GSequenceDiagram.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_GSEQUENCEDIAGRAM_H
#define DIAGRAMEDITOR_GSEQUENCEDIAGRAM_H

#include "GraphicsScene.h"
#include "ClassDiagram.h"
#include "GTimeline.h"
#include <QFrame>
#include <QListWidget>

/**
 * @brief Graphic representation of the sequence diagram tab
 */
class GSequenceDiagram: public QObject{
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param scene GraphicsScene to draw a diagram in
     * @param model UML diagram model
     * @param classDiagram UML source where the classes are from
     * @param settings GUI panel with object settings
     */
    GSequenceDiagram(GraphicsScene *scene, SequenceDiagram *model, ClassDiagram *classDiagram, QFrame *settings);
    /**
     * Delete timelines in relation with a class, which will be deleted
     * @param classifier Classifier that is deleted after
     */
    void deleteBeforeClass(UMLClassifier *classifier);

signals:
    /**
     * @brief Signal timelines that a content was changed
     */
    void classContentUpdated();
    /**
     * @brief Signal messages about the position change
     */
    void updateMsgPos();

private:
    /** Class diagram model pointer */
    ClassDiagram *classDiagram;
    /** Sequence diagram model pointer */
    SequenceDiagram *sequanceDiagram;
    /** Panel frame with settings */
    QFrame *settings;
    /**
     * @brief Add settings data into settings frame
     */
    void addSettings();
    /** Vector with all timelines */
    std::vector<GTimeline*> gTimelines;
    /** Vector with all messages */
    std::vector<GMessage*> gMessages;
    /** Placing index counter */
    int index;
    /** Graphics scene pointer */
    GraphicsScene *scene;
    /**
     * @brief Get a classifier pointer by the instance name
     * @param instName Instance name
     * @return UMLClassifier class
     */
    UMLClassifier *getClassifByInst(QString instName);
    /**
     * @brief Add a message row in the list of all messages
     * @param qList ListWidget
     * @param gMsg GMessage object
     */
    void addMsgItem(QListWidget *qList, GMessage *gMsg);
    /** List of all messages */
    QListWidget *msgList;
    /**
     * @brief Update content after a message is deleted
     */
    void updateAfterMsgDelete();
    /**
     * @brief Delete a GTimeline object
     * @param tl Timeline to delete
     */
    void deleteTimeline(GTimeline *tl);

private slots:
    /**
     * @brief Slot to receive class updates
     */
    void onClassDiagramUpdated();
    /**
     * @brief Slot to add an GObject after a button press
     */
    void onAddPressed();
    /**
     * @brief Slot to update function list
     */
    void onFuncUpdate();
    /**
     * @brief Slot activated by the object delete
     */
    void onGTimelineDeleted();
    /**
     * @brief Slot to save (add) a new message
     */
    void onSaveMsg();
    /**
     * @brief Reorder messages when some of them is moved to a new index
     */
    void msgDropped(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    /**
     * @brief Slot to delete a message on double click
     */
    void onMsgDoubleClick(const QModelIndex &ix);
};

#endif //DIAGRAMEDITOR_GSEQUENCEDIAGRAM_H
