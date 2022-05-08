/**
 * @brief Declaration of GClassSettings class
 * Main GUI settings panel for class and interface objects
 *
 * @file GClassSettings.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef TESTEDITOR_CLASSSETTINGS_H
#define TESTEDITOR_CLASSSETTINGS_H

#include <QTreeWidget>
#include "GClassifier.h"

#define SAVE_BUTTON_STYLE "QPushButton { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"\
                              "stop:0 #6BD96A, stop:0.05 #2BA94A, stop:0.95 #0B892A, stop:1 #06690F); font-size: 30px; border-radius: 3px;}" \
                              "QPushButton:hover { background-color: #25A545;}"\
                              "QPushButton:pressed { background-color: #057525;}"
#define SELECTED_ROW_STYLE "QFrame {background-color: #229944}"
#define CATEGORY_HEADER_STYLE "QFrame { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"\
                              "stop:0 #50555A, stop:0.05 #35383D, stop:0.95 #33363B, stop:1 #152020); }"
#define DELETE_BUTTON_STYLE "QPushButton { color: #FF4444;}"
#define RELATION_PARENT_STYLE "QLabel { color: rgb(255,255,255,145); border: 1px solid rgb(0,0,0,70); border-radius: 2px;}"

#define RELATION_TYPES " ,0,1,*,0..1,0..*,1..*,◆,◇,◁"
#define INHERITANCE_SYMB "◁"
#define ACCESSIBILITIES "+,-,#,~"

#define GENERAL "General"
#define ATTRIBUTES "Attributes"
#define FUNCTIONS "Functions"
#define RELATIONS "Relations"

/**
 * @brief Graphic panel for a classifier settings
 */
class GClassSettings: public QObject {
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param tree QTreeWidget where the settings categories will be added
     * @param classDiagram ClassDiagram window model
     */
    explicit GClassSettings(QTreeWidget *tree, ClassDiagram *classDiagram);
    /**
     * @brief Hide categories
     */
    void hideContent();
    /**
     * @brief Load content to categories from an exact GClassifier
     * @param gClassifier Object with info to fill in settings
     */
    void loadContent(GClassifier *gClassifier);
    /** Classifier text title */
    QString title;

private:
    /** Tree for categories */
    QTreeWidget *tree;
    /**
     * @brief Add category with general info settings
     * @return Category as a tree child
     */
    QTreeWidgetItem *addCategoryGeneral();
    /**
     * @brief Add category with attribute info settings
     * @return Category as a tree child
     */
    QTreeWidgetItem *addCategoryAttributes();
    /**
     * @brief Add category with function info settings
     * @return Category as a tree child
     */
    QTreeWidgetItem *addCategoryFunctions();
    /**
     * @brief Add category with relation info settings
     * @return Category as a tree child
     */
    QTreeWidgetItem *addCategoryRelations();
    /**
     * @brief Add item with save button
     * @return Category as a tree child
     */
    QTreeWidgetItem *addSavePanel();
    /** Vector with all current categories */
    std::vector<QTreeWidgetItem*> categories;
    /**
     * @brief Add a child row with add and remove button
     * @param category tree child to put in
     */
    void addDeleteAndAddRow(QTreeWidgetItem *category);
    /**
     * @brief Add the same top header for each category with add-remove buttons
     * @param name Category name
     * @return
     */
    QTreeWidgetItem *addCategoryHeader(const QString name);
    /** UML model */
    ClassDiagram *classDiagram;

public:
signals:
    /**
     * @brief Notify GClassifier about 'Save' click
     */
    void contentSaved();
    /**
     * @brief Notification when a property row was deleted
     */
    void contentDeleted();

private slots:
    /**
     * @brief Add a universal attribute GUI row to the category tree widget
     * @param obj Category widget
     * @param access Accessibility sign
     * @param type Type of the variable
     * @param name Name of the variable
     */
    void addRow(QWidget *obj, const QString &access = QString(), const QString &type = QString(), const QString &name = QString());
    /**
     * @brief Add a relation row with specific input boxes
     * @param obj Category widget
     * @param dst Target classifier name of the relation
     * @param srcMsg Src sign
     * @param dstMsg Dst sign
     */
    void addRelationRow(QWidget *obj, const QString &dst = QString(), const QString &srcMsg = QString(), const QString &dstMsg = QString());
    /**
     * @brief Add a function setting row
     * @param obj Category widget
     * @param access Accessibility sign
     * @param ret Return type of the function
     * @param name Name of the variable
     */
    void addFuncRow(QWidget *obj, const QString &access  = QString(), const QString &ret = QString(),
                    const QString &name = QString(), const QString &params = QString());
    /**
     * @brief Delete a setting row which is selected
     */
    void deleteRow();
    /**
     * @brief Inform about row deletion
     */
    void selectionChanged();
    /**
     * Receive a save signal
     */
    void saveContent();

private:
    /** Current selected GClassifier object */
    GClassifier *selectedGClassifier;
    /**
     * @brief Fill a function setting properly with splitting parameters
     * @param attr Exact operation that is being filled in
     * @param category Tree parent for a row
     * @param index Parent item index
     */
    void fillOperation(UMLOperation *attr, QTreeWidgetItem *category, int index);
    /**
     * @brief Load an operation from a UML model
     * @param category Tree widget to put in
     * @param a UML operation model with data
     */
    void loadOperation(QTreeWidgetItem *category, UMLOperation *a);
};

#endif //TESTEDITOR_CLASSSETTINGS_H