/**
 * @brief Declaration of DeleteObjectPopUp class
 * Dialog window that open when a user needs to remove a class but sequence diag. object exists
 *
 * @file DeleteObjectPopUp.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_DELETEOBJECTPOPUP_H
#define DIAGRAMEDITOR_DELETEOBJECTPOPUP_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class DeleteObjectPopUp; }
QT_END_NAMESPACE

/**
 * @brief Dialog window to task when removing class with sequence diagram objects
 */
class DeleteObjectPopUp : public QDialog {
Q_OBJECT

public:
    explicit DeleteObjectPopUp(QWidget *parent = nullptr);
    /**
     * @brief Public info if remove both was clicked
     */
    bool removeClicked();

    ~DeleteObjectPopUp() override;

private:
    Ui::DeleteObjectPopUp *ui;
    /** Info if remove both was clicked*/
    bool isRemove;

private slots:
    /**
     * @brief Slot to catch cancel click
     */
    void onCancel();
    /**
     * @brief Slot to catch remove click
     */
    void onRemove();
};


#endif //DIAGRAMEDITOR_DELETEOBJECTPOPUP_H
