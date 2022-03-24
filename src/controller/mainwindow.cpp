/**
 * Main window controller
 *
 * @authors Hung Do (xdohun00@stud.fit.vutbr.cz)
 *          David Kedra (xkedra00@stud.fit.vutbr.cz)
 */

#include "mainwindow.h"
#include "../view/ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

/* mainwindow.cpp */