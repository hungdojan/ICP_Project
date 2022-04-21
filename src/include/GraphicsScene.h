//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_GRAPHICSSCENE_H
#define DIAGRAMEDITOR_GRAPHICSSCENE_H

#include <QGraphicsScene>

class GraphicsScene: public QGraphicsScene {
public:
    GraphicsScene(QGraphicsView *view, QObject *parent = nullptr);
protected:
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);
private:
    QGraphicsView *view;
};

#endif //DIAGRAMEDITOR_GRAPHICSSCENE_H
