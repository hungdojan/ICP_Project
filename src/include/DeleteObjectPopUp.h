//
// Created by darbix on 5.5.22.
//

#ifndef DIAGRAMEDITOR_DELETEOBJECTPOPUP_H
#define DIAGRAMEDITOR_DELETEOBJECTPOPUP_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class DeleteObjectPopUp; }
QT_END_NAMESPACE

class DeleteObjectPopUp : public QDialog {
Q_OBJECT

public:
    explicit DeleteObjectPopUp(QWidget *parent = nullptr);
    bool isRemove;
    bool removeClicked();

    ~DeleteObjectPopUp() override;

private:
    Ui::DeleteObjectPopUp *ui;
private slots:
    void onCancel();
    void onRemove();
};


#endif //DIAGRAMEDITOR_DELETEOBJECTPOPUP_H
