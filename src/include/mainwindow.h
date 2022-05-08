/**
 * Main window header
 *
 * @file mainwindow.h
 * @date 07/05/2022
 * @authors Hung Do (xdohun00@stud.fit.vutbr.cz)
 *          David Kedra (xkedra00@stud.fit.vutbr.cz)
 */

#ifndef DIAGRAMEDITOR_MAINWINDOW_H
#define DIAGRAMEDITOR_MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTreeWidget>
#include <QSignalMapper>
#include "GraphicsScene.h"
#include "GClassDiagram.h"
#include "GSequenceDiagram.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    QTreeWidget *getCategoryTree();
    std::vector<GSequenceDiagram*> gSequenceDiagrams;
    void createSequenceDiagram(SequenceDiagram *model);

private slots:
    void clickedDiagram(QAction *a);
    void deleteDiagram();
    void newClassDiagram();
    void loadClassDiagram();
    void saveClassDiagram();

private:
    static int index;
    Ui::MainWindow *ui;
    GraphicsScene *scene;
    GClassDiagram *gClassDiagram;
    ClassDiagram *classDiagram;
    QSignalMapper *mapper;
};


#endif //DIAGRAMEDITOR_MAINWINDOW_H
