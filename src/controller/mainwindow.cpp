/**
 * Main window controller
 *
 * @file mainwindow.cpp
 * @date 07/05/2022
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
#include <QSignalMapper>
#include "JsonParser.h"
#include "CommandBuilder.h"

int MainWindow::index = 1;
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    mapper = nullptr;

    scene = new GraphicsScene(ui->graphicsView, this);
    ui->graphicsView->setScene(scene);

    classDiagram = new ClassDiagram("MyClassDiagram");
    gClassDiagram = new GClassDiagram(scene, classDiagram, this);
    gClassDiagram->loadSequenceDiagram();

    connect(ui->addClassButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifier()));
    connect(ui->addInterfaceButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifierInterface()));
    connect(ui->menuDiagram, SIGNAL(triggered(QAction *)), this, SLOT(clickedDiagram(QAction *)));
    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newClassDiagram()));
    connect(ui->actionLoad_from, SIGNAL(triggered()), this, SLOT(loadClassDiagram()));
    connect(ui->actionSave_to, SIGNAL(triggered()), this, SLOT(saveClassDiagram()));
    connect(ui->actionRedo, SIGNAL(triggered()), &CommandBuilder::get_commander(), SLOT(redoSlot()));
    connect(ui->actionUndo, SIGNAL(triggered()), &CommandBuilder::get_commander(), SLOT(undoSlot()));
}

MainWindow::~MainWindow() {
    delete ui;
    delete scene;
    delete mapper;
}
QTreeWidget *MainWindow::getCategoryTree(){
    return ui->treeWidget;
}

void MainWindow::deleteDiagram() {
    ui->menuBar->setHidden(true);
    ui->menuBar->setHidden(false);

    if(ui->diagramsTabs->currentIndex() != 0) {
        QString name{ui->diagramsTabs->tabText(ui->diagramsTabs->currentIndex())};
        classDiagram->removeSequenceDiagram(name.toStdString());
        ui->diagramsTabs->removeTab(ui->diagramsTabs->currentIndex());
    }
}

void MainWindow::clickedDiagram(QAction *a){
    if(a->text() == "Delete"){
        deleteDiagram();
        return;
    }

    QString tabName = "SequenceDiagram" + QString::number(MainWindow::index++);
    auto sequenceDiagramModel = classDiagram->addSequenceDiagram(tabName.toStdString());
    createSequenceDiagram(sequenceDiagramModel);
}

void MainWindow::saveClassDiagram() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save file"), QDir::currentPath(), "JSON file (*.json)");
    if (filePath == "")
        return;

    if (!filePath.endsWith(".json")) {
        filePath += ".json";
    }
    JsonParser::saveToFile(*classDiagram, filePath.toStdString());
}

void MainWindow::loadClassDiagram() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Load file"), QDir::currentPath(), tr("JSON file (*.json)"));
    if (filePath == "")
        return;

    for (int i = ui->diagramsTabs->count(); i >= 1; i--)
        ui->diagramsTabs->removeTab(i);
    delete scene;
    delete gClassDiagram;
    delete classDiagram;
    classDiagram = new ClassDiagram("temporary class diagram");
    JsonParser::initFromFile(*classDiagram, filePath.toStdString());

    index = static_cast<int>(classDiagram->sequenceDiagrams().size());

    scene = new GraphicsScene(ui->graphicsView, this);

    ui->graphicsView->setScene(scene);

    gClassDiagram = new GClassDiagram(scene, classDiagram, this);
    connect(ui->addClassButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifier()));
    connect(ui->addInterfaceButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifierInterface()));
    gClassDiagram->loadSequenceDiagram();
}

void MainWindow::newClassDiagram() {
    // clean up and set default values
    for (int i = ui->diagramsTabs->count(); i >= 1; i--)
        ui->diagramsTabs->removeTab(i);
    delete scene;
    delete gClassDiagram;
    delete classDiagram;
    index = 1;

    scene = new GraphicsScene(ui->graphicsView, this);
    ui->graphicsView->setScene(scene);

    classDiagram = new ClassDiagram("MyClassDiagram");
    gClassDiagram = new GClassDiagram(scene, classDiagram, this);
    connect(ui->addClassButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifier()));
    connect(ui->addInterfaceButton, SIGNAL(pressed()), gClassDiagram, SLOT(addClassifierInterface()));
    gClassDiagram->loadSequenceDiagram();
}

void MainWindow::createSequenceDiagram(SequenceDiagram *model) {
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

    ui->diagramsTabs->addTab(seq, QString::fromStdString(model->name()));
    GSequenceDiagram *gSeqDiag = new GSequenceDiagram(seqScene, model, classDiagram, rightPanelSettings);
    gSequenceDiagrams.push_back(gSeqDiag);

    connect(gClassDiagram, SIGNAL(classDiagramUpdated()), gSeqDiag, SLOT(onClassDiagramUpdated()));

}

/* mainwindow.cpp */