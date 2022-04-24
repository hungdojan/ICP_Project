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
#include "UMLRelation.h"
#include "GRelation.h"

GClassDiagram::GClassDiagram(GraphicsScene *scene, ClassDiagram *model) : scene_{scene}, classDiagramModel{model} {
    gClassSettings = new GClassSettings(((MainWindow*)scene->parent())->getCategoryTree(), classDiagramModel);
}

void GClassDiagram::addClassifier() {
    addGClassifier(new GClassifier("class"+std::to_string(name_index++), 0, 0, 100, 150, classDiagramModel));
}

void GClassDiagram::addClassifierInterface() {
    addGClassifier(new GClassifier("interface"+std::to_string(name_index++), 0, 0, 100, 150, classDiagramModel, true));
}
void GClassDiagram::addGClassifier(GClassifier *gRect){
    scene_->addItem(gRect);
    connect(gRect, SIGNAL(gClassifierSelectionChanged()), this, SLOT(onGClassifierSelectionChanged()));
    connect(gRect, SIGNAL(gClassifierContentChanged()), this, SLOT(onGClassifierContentChanged()));

    gClassifiers.push_back(gRect);
}

void GClassDiagram::onGClassifierSelectionChanged(){
    if(((GClassifier*)sender())->isSelected())
        gClassSettings->hideContent();
    else
        gClassSettings->loadContent((GClassifier*)sender());
}

void GClassDiagram::onGClassifierContentChanged(){
    auto src = (GClassifier*)sender();

    for(auto r: src->umlClassifier->relations()){
        if(r->src() != src->umlClassifier) // Only GRelations from src view are saved, (not twice)
            continue;

        // Find the GClassifier with which the src is in uml relation
        GClassifier *dst = nullptr;
        for(auto d: gClassifiers){
            // If the d is a classifier in relation with src
            if(d->umlClassifier == r->dst()) {
                dst = d;

                // Check if GRelation already exists
                for (auto gR: gRelations) {
                    if ((GClassifier *)gR->getSrc() == src && (GClassifier *)gR->getDst() == dst ||
                        (GClassifier *)gR->getDst() == src && (GClassifier *)gR->getSrc() == dst) {
                        // Update relation symbols
                        gR->updateContent(
                                gR->getSrc() == src ? QString::fromStdString(r->srcMsg()) : QString::fromStdString(r->dstMsg()),
                                gR->getDst() == src ? QString::fromStdString(r->srcMsg()) : QString::fromStdString(r->dstMsg()));
                        dst = nullptr;
                    }
                }
            }
        }
        if(dst) {
            auto newRel = new GRelation(src, dst, QString::fromStdString(r->srcMsg()), QString::fromStdString(r->dstMsg()));
            gRelations.push_back(newRel);
        }
    }
//     If GRelation exists but UML not
    for(auto gR: gRelations){
//        GClassifier *inRelItem = gR->getSrc() != src? src: (GClassifier *)gR->getDst(); // Item in relation with src
        GClassifier *inRelItem = nullptr;
        if(gR->getSrc() == src)
            inRelItem = (GClassifier*)gR->getDst();
        else if(gR->getDst() == src)
            inRelItem = (GClassifier*)gR->getSrc();
//        if(!src->umlClassifier->inRelationWith(inRelItem->umlClassifier)) {

        if(inRelItem && !src->umlClassifier->inRelationWith(inRelItem->umlClassifier)){
            gRelations.erase(std::remove(gRelations.begin(), gRelations.end(), gR), gRelations.end());
            delete gR;
        }
    }
}