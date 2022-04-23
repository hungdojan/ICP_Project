//
// Created by darbix on 21.4.22.
//

#include "GClassDiagram.h"
#include "GraphicsScene.h"
#include "ClassDiagram.h"
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include "GClassSettings.h"
#include "mainwindow.h"

GClassDiagram::GClassDiagram(GraphicsScene *scene, ClassDiagram *model) : scene_{scene}, classDiagramModel{model} {
    gClassSettings = new GClassSettings(((MainWindow*)scene->parent())->getCategoryTree(), classDiagramModel);
}

void GClassDiagram::addClassifier() {
    GClassifier *rect = new GClassifier("class"+std::to_string(name_index++), 0, 0, 100, 150, classDiagramModel);
    scene_->addItem(rect);
    connect(rect, SIGNAL(gClassifierSelectionChanged()), this, SLOT(onGClassifierSelectionChanged()));
    gClassifiers.push_back(rect);

    // Select after creation todo more retangles were selected...
//    rect->setSelected(true);
//    gClassSettings->loadContent(rect);
}

void GClassDiagram::addClassifierInterface() {
    GClassifier *rect = new GClassifier("interface"+std::to_string(name_index++), 0, 0, 100, 150, classDiagramModel, true);
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