//
// Created by darbix on 5.5.22.
//

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