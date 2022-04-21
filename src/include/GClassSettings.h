#ifndef TESTEDITOR_CLASSSETTINGS_H
#define TESTEDITOR_CLASSSETTINGS_H

#include <QTreeWidget>
//#include "UMLClassifier.h"
#include "GClassifier.h"

class GClassSettings: public QObject {
Q_OBJECT
public:
    GClassSettings(QTreeWidget *tree);
//    void loadCategories();
    void hideContent();
    void loadContent(GClassifier *gClassifier);
    QString title;

private:
    QTreeWidget *tree;
    QTreeWidgetItem *addCategoryGeneral();
    QTreeWidgetItem *addCategoryAttributes();
    QTreeWidgetItem *addCategoryFunctions();
    QTreeWidgetItem *addCategoryRelations();
    QTreeWidgetItem *addSavePanel();
    std::vector<QTreeWidgetItem*> categories;
    void addDeleteAndAddRow(QTreeWidgetItem *category);
    QTreeWidgetItem *addCategoryHeader(QString name);

public:
signals:
    void contentSaved();
private slots:
    void addRow(QWidget *);
    void deleteRow();
    void selectionChanged();
    void saveContent();
private:
    GClassifier *selectedGClassifier;
};

#endif //TESTEDITOR_CLASSSETTINGS_H