//
// Created by darbix on 21.4.22.
//

#include "GClassDiagram.h"
#include "GraphicsScene.h"
#include "ClassDiagram.h"
#include <QDebug>
#include "GClassSettings.h"
#include "mainwindow.h"

GClassDiagram::GClassDiagram(GraphicsScene *scene, ClassDiagram *model) : scene_{scene}, classDiagramModel{model} {
    gClassSettings = new GClassSettings(((MainWindow*)scene->parent())->getCategoryTree());
}

void GClassDiagram::addClassifier() {
    GClassifier *rect = new GClassifier(0, 0, 100, 150);
    scene_->addItem(rect);

    connect(rect, SIGNAL(gClassifierSelectionChanged()), this, SLOT(onGClassifierSelectionChanged()));
    gClassifiers.push_back(rect);
}

void GClassDiagram::onGClassifierSelectionChanged(){
    if(((GClassifier*)sender())->isSelected())
        gClassSettings->hideContent();
    else
        gClassSettings->loadContent((GClassifier*)sender());
}