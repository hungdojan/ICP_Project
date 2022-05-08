/**
 * @brief Declaration of GraphicsScene class
 * QGraphicsScene inheritor scene which is zoomable
 *
 * @file GraphicsScene.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_GRAPHICSSCENE_H
#define DIAGRAMEDITOR_GRAPHICSSCENE_H

#include <QGraphicsScene>

/**
 * @brief Zoomable QGraphicsScene
 */
class GraphicsScene: public QGraphicsScene {
public:
    /**
     * @brief Constructor
     * @param view QGraphicsView to fit in
     * @param parent Parent object
     */
    GraphicsScene(QGraphicsView *view, QObject *parent = nullptr);
protected:
    /**
     * @brief Base wheelEvent reaction to zoom the scene
     */
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);
private:
    /** Parent view object */
    QGraphicsView *view;
};

#endif //DIAGRAMEDITOR_GRAPHICSSCENE_H
