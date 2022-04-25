#ifndef TESTEDITOR_CLASSSETTINGS_H
#define TESTEDITOR_CLASSSETTINGS_H

#include <QTreeWidget>
//#include "UMLClassifier.h"
#include "GClassifier.h"

class GClassSettings: public QObject {
Q_OBJECT
public:
    explicit GClassSettings(QTreeWidget *tree, ClassDiagram *classDiagram);
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
    QTreeWidgetItem *addCategoryHeader(const QString name);
    ClassDiagram *classDiagram;

public:
signals:
    void contentSaved();
    void contentDeleted();
private slots:
    void addRow(QWidget *, const QString &access = QString(), const QString &type = QString(), const QString &name = QString());
    void addRelationRow(QWidget *, const QString &dst = QString(), const QString &srcMsg = QString(), const QString &dstMsg = QString());
    void addFuncRow(QWidget *obj, const QString &access  = QString(), const QString &ret = QString(),
                    const QString &name = QString(), const QString &params = QString());
    void deleteRow();
    void selectionChanged();
    void saveContent();
private:
    GClassifier *selectedGClassifier;
    void fillOperation(UMLOperation *attr, QTreeWidgetItem *category, int index);
    void loadOperation(QTreeWidgetItem *category, UMLOperation *a);
};

#endif //TESTEDITOR_CLASSSETTINGS_H