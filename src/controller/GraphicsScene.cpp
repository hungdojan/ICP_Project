//
// Created by darbix on 21.4.22.
//

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