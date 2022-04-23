/**
 * @brief Main entry point of program.
 *
 * @file main.cpp
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow m;
    m.show();
    return QApplication::exec();
}
