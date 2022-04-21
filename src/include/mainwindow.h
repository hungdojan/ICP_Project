/**
 * Main window header
 *
 * @authors Hung Do (xdohun00@stud.fit.vutbr.cz)
 *          David Kedra (xkedra00@stud.fit.vutbr.cz)
 */

#ifndef DIAGRAMEDITOR_MAINWINDOW_H
#define DIAGRAMEDITOR_MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTreeWidget>
#include "GraphicsScene.h"
#include "GClassDiagram.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    QTreeWidget *getCategoryTree();

//private slots:
//    void addClass();

private:
    Ui::MainWindow *ui;
    GraphicsScene *scene;
    GClassDiagram *gClassDiagram;
};


#endif //DIAGRAMEDITOR_MAINWINDOW_H
