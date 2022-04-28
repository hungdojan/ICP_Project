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
#include "ClassDiagram.h"
#include <QDebug>
#include <QComboBox>
#include "SequenceDiagram.h"
#include <QFileDialog>
#include "JsonParser.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    scene = new GraphicsScene(ui->graphicsView, this);
    ui->graphicsView->setScene(scene);

    classDiagram = new ClassDiagram("MyClassDiagram");
    gClassDiagram = new GClassDiagram(scene, classDiagram);

    connect(ui->addClassButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifier()));
    connect(ui->addInterfaceButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifierInterface()));
    connect(ui->menuDiagram, SIGNAL(triggered(QAction *)), this, SLOT(clickedDiagram(QAction *)));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newClassDiagram()));
    connect(ui->actionLoad_from, SIGNAL(triggered()), this, SLOT(loadClassDiagram()));
    connect(ui->actionSave_to, SIGNAL(triggered()), this, SLOT(saveClassDiagram()));

    clickedDiagram(new QAction()); // TODO remove
}

void MainWindow::onTests() {
//    qDebug() << "todo";
}

MainWindow::~MainWindow() {
    delete ui;
    delete scene;
}
QTreeWidget *MainWindow::getCategoryTree(){
    return ui->treeWidget;
}

void MainWindow::deleteDiagram() {
    ui->menuBar->setHidden(true);
    ui->menuBar->setHidden(false);

    if(ui->diagramsTabs->currentIndex() != 0)
        ui->diagramsTabs->removeTab(ui->diagramsTabs->currentIndex());
}

void MainWindow::clickedDiagram(QAction *a){
    if(a->text() == "Delete"){
        deleteDiagram();
        return;
    }

    QWidget *seq = new QWidget();
    QHBoxLayout *hLayout = new QHBoxLayout(seq);
    hLayout->setContentsMargins(0,0,0,0);
    QGraphicsView *gView = new QGraphicsView();
    gView->setStyleSheet("QGraphicsView { background-color: #FFFFFF; }");
    GraphicsScene *seqScene = new GraphicsScene(gView, this);
    gView->setScene(seqScene);
    hLayout->addWidget(gView);
    hLayout->setSpacing(0);

    QFrame *rightPanelSettings = new QFrame();
    hLayout->addWidget(rightPanelSettings);

    ui->diagramsTabs->addTab(seq, "seq1");
    classDiagram->addSequenceDiagram("seq1");
    GSequenceDiagram *gSeqDiag = new GSequenceDiagram(seqScene, "seq1", classDiagram, rightPanelSettings);
    gSequenceDiagrams.push_back(gSeqDiag);

    connect(gClassDiagram, SIGNAL(classDiagramUpdated()), gSeqDiag, SLOT(onClassDiagramUpdated()));
}

void MainWindow::saveClassDiagram() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save file"), QDir::currentPath(), ".json");
    if (filePath == "")
        return;

    qDebug() << filePath;
    if (!filePath.endsWith(".json")) {
        filePath += ".json";
    }
    JsonParser::saveToFile(*classDiagram, filePath.toStdString());
}

void MainWindow::loadClassDiagram() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), QDir::currentPath(), tr("JSON file (*.json)"));
    if (filePath == "")
        return;

    // TODO: warning dialog popup
    // ask user to save current work
    // exit if user cancel
    qDebug() << filePath;
    // TODO: clean current and load from file
    delete scene;
    delete gClassDiagram;
    delete classDiagram;
    classDiagram = new ClassDiagram("temporary class diagram");
    JsonParser::initFromFile(*classDiagram, filePath.toStdString());

    // TODO: make better code

    scene = new GraphicsScene(ui->graphicsView, this);
    ui->graphicsView->setScene(scene);

    gClassDiagram = new GClassDiagram(scene, classDiagram);
    connect(ui->addClassButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifier()));
    connect(ui->addInterfaceButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifierInterface()));
    connect(ui->menuDiagram, SIGNAL(triggered(QAction *)), this, SLOT(clickedDiagram(QAction *)));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newClassDiagram()));
}

void MainWindow::newClassDiagram() {
    qDebug() << ui->diagramsTabs->children().count();
    delete scene;
    delete gClassDiagram;
    delete classDiagram;

    scene = new GraphicsScene(ui->graphicsView, this);
    ui->graphicsView->setScene(scene);

    classDiagram = new ClassDiagram("MyClassDiagram");
    gClassDiagram = new GClassDiagram(scene, classDiagram);
    connect(ui->addClassButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifier()));
    connect(ui->addInterfaceButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifierInterface()));
    connect(ui->menuDiagram, SIGNAL(triggered(QAction *)), this, SLOT(clickedDiagram(QAction *)));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newClassDiagram()));
    // clickedDiagram(new QAction()); // TODO remove
}

/* mainwindow.cpp */