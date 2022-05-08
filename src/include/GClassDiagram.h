/**
 * @brief Declaration of GClassDiagram class
 * Main graphics for UML class diagram
 *
 * @file GClassDiagram.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_GCLASSDIAGRAM_H
#define DIAGRAMEDITOR_GCLASSDIAGRAM_H

#include "GClassifier.h"
#include "GraphicsScene.h"
#include "ClassDiagram.h"
#include "GClassSettings.h"
#include "GSequenceDiagram.h"

class MainWindow;

/**
 * @brief Graphic controller for a ClassDiagram model
 */
class GClassDiagram: public QObject{
Q_OBJECT
public:
    GClassDiagram() =delete;
    /**
     * @brief GClassDiagram constructor
     * @param scene GraphicsScene used in QGraphicsView
     * @param model UML class diagram model
     * @param mainWindow MainWindow object
     */
    GClassDiagram(GraphicsScene *scene, ClassDiagram *model, MainWindow *mainWindow);
//    void setModel(ClassDiagram *model);
//    const ClassDiagram *model() const;
//    void scene(GraphicsScene *scene);
//    const GraphicsScene *scene() const;
    /**
     * @brief Method to load content to sequence diagrams
     */
    void loadSequenceDiagram();

signals:
    /**
     * @brief Signal to notify GClassifier about using UNDEF due to removed classifier
     */
    void updateUndefTypes();
    /**
     * @brief Signal to notify outher diagrams and MainWindow about changes
     */
    void classDiagramUpdated();

private slots:
    /**
     * @brief Slot to invoke addition of Class to the scene
     */
    void addClassifier();
    /**
     * @brief Slot to invoke addition of Interface to the scene
     */
    void addClassifierInterface();
    /**
     * @brief Slot to react to a click on a GClassifier
     */
    void onGClassifierSelectionChanged();
    /**
     * @brief SLot to get notification about changes from GClassifiers
     */
    void onGClassifierContentChanged();
    /**
     * @brief  Slot to get update about GClassifier deletion
     */
    void onGClassifierDeleted();

private:
    /** Pointer to a ClassDiagram instance */
    ClassDiagram *classDiagramModel;
    /** Pointer to a scene */
    GraphicsScene *scene_;
    /** Vector of all GClassifiers in the scene */
    std::vector<GClassifier*> gClassifiers;
    /** Vector of all GRelations in the scene */
    std::vector<GRelation*> gRelations;
    /** Pointer to MainWindow object */
    MainWindow *mainWindow;
    /** GUI panel with classifier settings */
    GClassSettings *gClassSettings;
    /** Index for generating classifier names */
    int name_index{0};
    /**
     * Add created GClassifier object to the scene and vector
     * @param g
     */
    void addGClassifier(GClassifier *g);
    /**
     * @brief Creates a string with the classifier name ordered with index
     * @param classifierName
     * @return Indexed name of the classifier
     */
    std::string getIndexed(std::string classifierName);
};

#endif //DIAGRAMEDITOR_GCLASSDIAGRAM_H
