/**
 * @brief Dialog window that open when a user needs to remove a class but sequence diag. object exists
 *
 * @file DeleteObjectPopUp.cpp
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#include "DeleteObjectPopUp.h"
#include "../view/ui_DeleteObjectPopUp.h"
#include <QDebug>

DeleteObjectPopUp::DeleteObjectPopUp(QWidget *parent) :
        QDialog(parent), ui(new Ui::DeleteObjectPopUp) {
    ui->setupUi(this);

    connect(ui->buttonCancel, SIGNAL(pressed()), this, SLOT(onCancel()));
    connect(ui->buttonRemove, SIGNAL(pressed()), this, SLOT(onRemove()));
    isRemove = false;
}

DeleteObjectPopUp::~DeleteObjectPopUp() {
    delete ui;
}

void DeleteObjectPopUp::onCancel(){
    isRemove = false;
    close();
}

void DeleteObjectPopUp::onRemove(){
    isRemove = true;
    close();
}
bool DeleteObjectPopUp::removeClicked(){
    return isRemove;
}