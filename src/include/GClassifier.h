/**
 * @brief Declaration of GClassifier class
 * General graphic class for rectangle objects UMLInterface or UMLClass
 *
 * @file GClassifier.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_GCLASIFIER_H
#define DIAGRAMEDITOR_GCLASIFIER_H

#include <QGraphicsRectItem>
#include "GRelation.h"
#include "UMLClassifier.h"
#include "GText.h"
#include "ClassDiagram.h"

/**
 * @brief Graphic rectangle item for a Classifier model
 */
class GClassifier: public QObject, public QGraphicsRectItem{
Q_OBJECT
public:
    /**
     * @brief Constructor to create a blank new GClassifier
     * @param name Name of the classifier
     * @param x X position
     * @param y Y position
     * @param width Width
     * @param height Height
     * @param classDiagram UML ClassDiagram model
     * @param isInterface Bool if it is an interface
     * @param parent Graphics parent
     */
    GClassifier(std::string name, qreal x, qreal y, qreal width, qreal height, ClassDiagram *classDiagram, bool isInterface = false, QGraphicsItem *parent = nullptr);
    /**
     * @brief Constructor to create a GClassifier from the loaded UML
     * @param model Classifier UML model
     * @param x X position
     * @param y Y position
     * @param width Width
     * @param height Height
     * @param classDiagram ClassDiagram UML model
     * @param parent Graphics parent
     */
    GClassifier(UMLClassifier *model, qreal x, qreal y, qreal width, qreal height, ClassDiagram *classDiagram, QGraphicsItem *parent = nullptr);
    ~GClassifier();
    /** QObject itemChange implementation */
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    /** UML classifier model*/
    UMLClassifier *umlClassifier;
    /** Overridden mousePressEvent */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    /** Overridden mouseReleaseEvent */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    /**
     * @brief Signal to notify GClassDiagram about position change
     */
    void gClassifierPositionChanged();
    /**
     * @brief Signal to notify GClassDiagram about selection change
     */
    void gClassifierSelectionChanged();
    /**
     * @brief Signal to notify children about TextChange
     * @return Current width
     */
    qreal gTextChanged();
    /**
     * @brief Signal to cascade text center
     */
    void centerText();
    /**
     * Signal to notify about content changes
     */
    void gClassifierContentChanged();
    /**
     * Signal to notify about GClassifier deletion
     */
    void gClassifierDeleted();

private:
    /** Title GText graphics box */
    GText *title;
    /** Interface or Class text graphics box */
    GText *classificType;
    /** Vector of all attributes */
    std::vector<GText*> gTextAttributes;
    /** Vector of all relations, where this classifier is a source*/
    std::vector<GRelation*> gRelations;
    /** Bool if the Classifier is an interface */
    bool isInterface;
    /** ClassDiagram UML model */
    ClassDiagram *classDiagram;
    /**
     * @brief Load attribute and function items to a GClassifier body
     */
    void loadAttributes();
    /** Attributes outline rectangle */
    QGraphicsRectItem *attribRect;
    /** Title outline rectangle */
    QGraphicsRectItem *titleRect;
    /**
     * @brief Resize all children rectangles due to a size change
     */
    void resizeRectangles();
    qreal width;
    qreal height;
    qreal x;
    qreal y;
    /** keyReleaseEvent implementation */
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    /**
     * @brief Slot to react to the 'Save'
     */
    void contentSaved();
    /**
     * @brief Slot to receive deletion signal
     */
    void contentDeleted();
    /**
     * @brief Slot to update when used class-type is removed
     */
    void onUpdateUndefTypes();

    /** Base class mouseMoveEvent implementation */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif //DIAGRAMEDITOR_GCLASIFIER_H
