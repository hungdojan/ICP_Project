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
#include "UMLInterface.h"
#include "CommandBuilder.h"

GClassDiagram::GClassDiagram(GraphicsScene *scene, ClassDiagram *model) : scene_{scene}, classDiagramModel{model} {
    gClassSettings = new GClassSettings(((MainWindow*)scene->parent())->getCategoryTree(), classDiagramModel);
    if (model == nullptr) {
        model = new ClassDiagram("");
    }
    // load classifiers
    for (auto classifier : model->classElements()) {
        if (dynamic_cast<UMLClass *>(classifier) != nullptr || dynamic_cast<UMLInterface *>(classifier) != nullptr) {
            addGClassifier(new GClassifier(classifier, classifier->x(), classifier->y(), 100, 150, classDiagramModel));
        }
    }
    // collection of already initialized relations
    std::vector<UMLRelation *> relationsUsed;

    for (auto classifier : model->classElements()) {
        // skip basic classifiers
        if (dynamic_cast<UMLClass *>(classifier) == nullptr && dynamic_cast<UMLInterface *>(classifier) == nullptr)
            continue;

        // iterate over relations of the classifier
        // when current relation is not found in relationUsed meaning that it wasn't initialized yet
        // new instance of GRelation is created and added to the list
        for (auto relation : classifier->relations()) {
            auto iter {std::find(relationsUsed.begin(), relationsUsed.end(), relation)};
            if (iter != relationsUsed.end())
                continue;
            auto srcGClassifier = *(std::find_if(gClassifiers.begin(), gClassifiers.end(),
                                                 [relation](GClassifier *gClassifier) { return relation->src() == gClassifier->umlClassifier; }));
            auto dstGClassifier = *(std::find_if(gClassifiers.begin(), gClassifiers.end(),
                                                 [relation](GClassifier *gClassifier) { return relation->dst() == gClassifier->umlClassifier; }));
            gRelations.push_back(new GRelation(srcGClassifier, dstGClassifier,
                                               QString::fromStdString(relation->srcMsg()), QString::fromStdString(relation->dstMsg())));
        }
    }
    CommandBuilder::get_commander().clear();
}

void GClassDiagram::addClassifier() {
    addGClassifier(new GClassifier(getIndexed("class"), 2*name_index, 2*name_index, 100, 150, classDiagramModel));
}

void GClassDiagram::addClassifierInterface() {
    addGClassifier(new GClassifier(getIndexed("interface"), 2*name_index, 2*name_index, 100, 150, classDiagramModel, true));
}

std::string GClassDiagram::getIndexed(std::string classifierName){
    std::string name = classifierName+std::to_string(name_index++);
    while(classDiagramModel->getClassifier(name)){
        name = classifierName+std::to_string(name_index++);
    }
    return name;
}

void GClassDiagram::addGClassifier(GClassifier *gRect){
    scene_->addItem(gRect);
    connect(gRect, SIGNAL(gClassifierSelectionChanged()), this, SLOT(onGClassifierSelectionChanged()));
    connect(gRect, SIGNAL(gClassifierContentChanged()), this, SLOT(onGClassifierContentChanged()));
    connect(gRect, SIGNAL(gClassifierDeleted()), this, SLOT(onGClassifierDeleted()));
    connect(this, SIGNAL(updateUndefTypes()), gRect, SLOT(onUpdateUndefTypes()));

    gClassifiers.push_back(gRect);
    // gRects cannot emit from constructor initialization
    emit classDiagramUpdated(); // todo was not there Notify SequenceDiagram
}

void GClassDiagram::onGClassifierSelectionChanged(){
    if(((GClassifier*)sender())->isSelected())
        gClassSettings->hideContent();
    else
        gClassSettings->loadContent((GClassifier*)sender());
}

void GClassDiagram::onGClassifierDeleted(){
    auto src = (GClassifier*)sender();

    for(int i = gRelations.size()-1; i >= 0; i--){
        if(dynamic_cast<GClassifier *>(gRelations.at(i)->getDst()) == src || dynamic_cast<GClassifier *>(gRelations.at(i)->getSrc()) == src) {
            auto gR = gRelations.at(i);
            gRelations.erase(gRelations.begin() + i);
            delete gR;
        }
    }

    gClassifiers.erase(std::find(gClassifiers.begin(), gClassifiers.end(), src));

    classDiagramModel->removeClassElement(src->umlClassifier);
    delete src->umlClassifier;
    delete src;

    gClassSettings->hideContent();
    emit updateUndefTypes();
    emit classDiagramUpdated(); // Notify SequenceDiagram
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
    // If GRelation exists but UML not
    for(auto gR: gRelations){
        GClassifier *inRelItem = nullptr;
        if(dynamic_cast<GClassifier *>(gR->getSrc()) == src)
            inRelItem = dynamic_cast<GClassifier *>(gR->getDst());
        else if(dynamic_cast<GClassifier *>(gR->getDst()) == src)
            inRelItem = dynamic_cast<GClassifier *>(gR->getSrc());

        if(inRelItem && !src->umlClassifier->inRelationWith(inRelItem->umlClassifier)){
            gRelations.erase(std::find(gRelations.begin(), gRelations.end(), gR), gRelations.end());
            delete gR;
        }
    }

    emit classDiagramUpdated(); // Notify SequenceDiagram
}