/**
 * Main window controller
 *
 * @authors Hung Do (xdohun00@stud.fit.vutbr.cz)
 *          David Kedra (xkedra00@stud.fit.vutbr.cz)
 */

#include "mainwindow.h"
#include "../view/ui_mainwindow.h"
#include "GraphicsScene.h"
#include "GClassifier.h"
#include "GRelation.h"
#include "GClassDiagram.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    scene = new GraphicsScene(ui->graphicsView, this);
    ui->graphicsView->setScene(scene);

    gClassDiagram = new GClassDiagram(scene);

    connect(ui->addClassButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifier()));

//
//    auto *rect1 = new GClassifier(-20, 20, 100, 150);
//    scene->addItem(rect1);
//    auto *rect2 = new GClassifier(200, 50, 100, 150);
//    scene->addItem(rect2);
//
//    auto *rel1 = new GRelation(rect1, rect2);
//    scene->addItem(rel1);
}

MainWindow::~MainWindow() {
    delete ui;
    delete scene;
}
QTreeWidget *MainWindow::getCategoryTree(){
    return ui->treeWidget;
}

/* mainwindow.cpp */