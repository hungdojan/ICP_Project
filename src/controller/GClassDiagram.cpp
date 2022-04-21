//
// Created by darbix on 21.4.22.
//

#include "GClassDiagram.h"
#include "GraphicsScene.h"

GClassDiagram::GClassDiagram(GraphicsScene *scene){
    this->scene = scene;
}

void GClassDiagram::addClassifier() {
    GClassifier *rect = new GClassifier(0, 0, 100, 150);
    scene->addItem(rect);
}