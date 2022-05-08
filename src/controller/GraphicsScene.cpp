/**
 * @brief QGraphicsScene inheritor scene which is zoomable
 *
 * @file GraphicsScene.cpp
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "GraphicsScene.h"
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsView>

GraphicsScene::GraphicsScene(QGraphicsView *view, QObject *parent): QGraphicsScene(parent){
    // QGraphicsView pointer
    this->view = view;
}

void GraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent){
    // Zoom
    if (wheelEvent->modifiers() & Qt::ControlModifier) {
        const QGraphicsView::ViewportAnchor anchor = view->transformationAnchor();
        view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        int angle = wheelEvent->delta();
        qreal factor = angle > 0 ? 1.025 : 0.985;
        view->scale(factor, factor);
        view->setTransformationAnchor(anchor);
    }
    QGraphicsScene::wheelEvent(wheelEvent);
}

