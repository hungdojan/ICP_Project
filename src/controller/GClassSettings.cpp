
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>
#include "GClassSettings.h"
#include <QLineEdit>
#include <QDebug>
#include <QString>
//#include "UMLClassifier.h"
#include "GClassifier.h"


#define SAVE_BUTTON_STYLE "QPushButton { background-color: #0B892A; font-size: 30px;}"
#define SELECTED_ROW_STYLE "QFrame {background-color: #229944}"
#define CATEGORY_HEADER_STYLE "QFrame { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"\
                              "stop:0 #50555A, stop:0.05 #35383D, stop:0.95 #33363B, stop:1 #152020); }"
#define DELETE_BUTTON_STYLE "QPushButton { color: #FF4444;}"


GClassSettings::GClassSettings(QTreeWidget *tree): QObject(){
    this->tree = tree;

    selectedGClassifier = nullptr;

    categories.push_back(addSavePanel());
    categories.push_back(addCategoryGeneral());
    categories.push_back(addCategoryAttributes());
    categories.push_back(addCategoryFunctions());
    categories.push_back(addCategoryRelations());

    hideContent();

    connect(tree, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
}

void GClassSettings::loadContent(GClassifier *gClassifier){
    //todo add attributes(e.g) rows to categories, when needed

    // Currently selected GClassifier
    selectedGClassifier = gClassifier;

    categories.at(0)->setHidden(false); // Save button panel

    for(int i = 1; i < categories.size(); ++i) {
        categories.at(i)->setHidden(false);
        QString categoryName = tree->itemWidget(categories.at(i), 0)->findChild<QLabel*>()->text();

        if(categoryName == "General") {
            tree->itemWidget(categories.at(i)->child(0),0)->findChild<QLineEdit *>()->setText(QString::fromStdString(gClassifier->umlClassifier->name())); // todo accessibleName ?
        }
    }
}

//void GClassSettings::loadCategories(){
//    // Show only those in vector that belongs the class
//    for(int i = 0; i < categories.size(); i++){
//        categories.at(i)->setHidden(false);
//    }
//}

void GClassSettings::hideContent() {
    tree->topLevelItem(0)->setHidden(true); // Save button panel

    // Hide all categories
    for(int i = 1; i < tree->topLevelItemCount(); i++){
        tree->topLevelItem(i)->setHidden(true);

        // todo starts at attributes, remove only blank???
        for(int k = 1; i > 1, k < tree->topLevelItem(i)->childCount(); k++) {
            tree->topLevelItem(i)->removeChild(tree->topLevelItem(i)->child(k));
            k--; // todo only for removing all children, number of them decreases -> also index
        }
    }
}

void GClassSettings::saveContent(){
    // todo better to bind to model data

    // [0] category Save Button
    // skip
    if(selectedGClassifier == nullptr)
        return;

    for(int i = 1; i < categories.size(); ++i) {
        QString categoryName = tree->itemWidget(categories.at(i), 0)->findChild<QLabel*>()->text();

        if(categoryName == "General") {
            selectedGClassifier->umlClassifier->setName(tree->itemWidget(categories.at(i)->child(0),0)->findChild<QLineEdit *>()->text().toStdString());
        }
    }
    // notify GClassifier
    connect(this, SIGNAL(contentSaved()), selectedGClassifier, SLOT(contentSaved()));
    emit contentSaved();
}

void GClassSettings::selectionChanged(){
    auto list = tree->selectedItems();
    // todo gets first, but must exist..
    QTreeWidgetItem *item = tree->selectedItems().at(0);
    if(item->parent() != NULL) {
        // Color all to default
        for(int i = 0; i < tree->topLevelItemCount(); ++i) {
            QTreeWidgetItem *topLvl = tree->topLevelItem(i);
            for (int r = 0; r < topLvl->childCount(); ++r) {
                tree->itemWidget(topLvl->child(r), 0)->setStyleSheet("");
            }
        }
        // Color selected non first row
        if(item->parent()->indexOfChild(item) != 0)
            tree->itemWidget(item, 0)->setStyleSheet(SELECTED_ROW_STYLE);
    }
}

void GClassSettings::deleteRow(){
    auto list = tree->selectedItems();
    //todo gets first, null?
    QTreeWidgetItem *item = tree->selectedItems().at(0);
    // Delete non-top item, which is not the first row with buttons todo !!! overit nazvem widget tree->itemWidget(categories.at(0)->child(1), 0)->findChild<QLineEdit*>("title_of_button")
    if(item != nullptr && item->parent() != nullptr && item->parent()->indexOfChild(item) >= 1) // todo dat title mimo category attributes
        item->parent()->removeChild(item);
}

void GClassSettings::addRow(QWidget *obj) {
    QTreeWidgetItem *category = (QTreeWidgetItem*)obj;
    // New attribute row
    auto *row = new QTreeWidgetItem();
    category->addChild(row);
    // Row Frame-Layout Content
    auto *rowFrame = new QFrame();
    QBoxLayout* rowLayout = new QHBoxLayout(rowFrame);
    rowLayout->setContentsMargins(9,0,9,0);

    auto *lineEditType = new QLineEdit();
    auto *lineEditContent = new QLineEdit();

    lineEditType->setPlaceholderText("type");
    lineEditType->setContentsMargins(0,0,0,0);
    lineEditType->setMaximumWidth(100);
    rowLayout->addWidget(lineEditType);
    lineEditContent->setPlaceholderText("name");
    lineEditContent->setContentsMargins(0,0,0,0);
    rowLayout->addWidget(lineEditContent);

    tree->setItemWidget(row, 0, rowFrame);
}

QTreeWidgetItem *GClassSettings::addSavePanel() {
    auto *savePanel = new QTreeWidgetItem();
    auto *savePanelFrame = new QFrame();
    savePanelFrame->setContentsMargins(0,0,0,0);
    QBoxLayout *savePanelLayout = new QHBoxLayout(savePanelFrame);
    savePanelLayout->setContentsMargins(0,0,0,0);
    auto *saveButton = new QPushButton("✔");
    saveButton->setStyleSheet(SAVE_BUTTON_STYLE);
    saveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    savePanelLayout->addWidget(saveButton);
    tree->addTopLevelItem(savePanel);
    tree->setItemWidget(savePanel, 0, savePanelFrame);

    connect(saveButton, SIGNAL(pressed()), this, SLOT(saveContent()));

    return savePanel;
}

void GClassSettings::addDeleteAndAddRow(QTreeWidgetItem *category) {
    // Add-new-row and Delete Buttons
    auto *topLine = new QTreeWidgetItem();
    category->addChild(topLine);
    auto *topLineFrame = new QFrame();
    QBoxLayout* topLineLayout = new QHBoxLayout(topLineFrame);

    auto *deleteButton = new QPushButton("❌");
    deleteButton->setFixedWidth(50);
    deleteButton->setStyleSheet(DELETE_BUTTON_STYLE);
    connect(deleteButton, SIGNAL(pressed()), this, SLOT(deleteRow()));

    auto *addButton = new QPushButton("➕");
    addButton->setFixedWidth(50);
    auto *mapper = new QSignalMapper(this);
    connect(addButton, SIGNAL(pressed()), mapper, SLOT(map()));
    mapper->setMapping(addButton, (QWidget*)category);
    connect(mapper, SIGNAL(mapped(QWidget*)), this, SLOT(addRow(QWidget*)));

    topLineLayout->addWidget(deleteButton, 1, Qt::AlignRight);
    topLineLayout->addWidget(addButton, 0, Qt::AlignRight);
    tree->setItemWidget(topLine, 0, topLineFrame);
}

QTreeWidgetItem *GClassSettings::addCategoryHeader(QString name){
    auto *category = new QTreeWidgetItem();
    tree->addTopLevelItem(category);
    // Category Frame-Layout Content
    auto *headerLineFrame = new QFrame();
    QBoxLayout* headerLineLayout = new QHBoxLayout(headerLineFrame);

    auto *label = new QLabel(name);
    label->setAlignment(Qt::AlignHCenter);
    label->setContentsMargins(0,2,0,2);
    label->setFont(QFont("Ubuntu", 12, QFont::Normal));
    label->setStyleSheet("QLabel { background-color: #00000000;}"); // Transparent label
    headerLineFrame->setStyleSheet(CATEGORY_HEADER_STYLE);
    headerLineLayout->addWidget(label);
    tree->setItemWidget(category, 0, headerLineFrame);

    return category;
}

QTreeWidgetItem *GClassSettings::addCategoryGeneral() {
    // Category 'General'
    auto *category = addCategoryHeader("General");

    // Title
    auto *row = new QTreeWidgetItem();
    category->addChild(row);
    // Row Frame-Layout Content
    auto *rowFrame = new QFrame();
    QBoxLayout* rowLayout = new QHBoxLayout(rowFrame);
    rowLayout->setContentsMargins(9,0,9,0);

    auto qLabelTitle = new QLabel("Title");
    qLabelTitle->setAccessibleName("QLabel_Title");
    rowLayout->addWidget(qLabelTitle);
    auto *lineEdit = new QLineEdit();
    lineEdit->setPlaceholderText("title");
    rowLayout->addWidget(lineEdit);
    tree->setItemWidget(row, 0, rowFrame);

    return category;
}

QTreeWidgetItem *GClassSettings::addCategoryAttributes() {
    // Category 'Attributes'
    auto *categoryAttributes = addCategoryHeader("Attributes");

    // Attach buttons 'X' and '+' row
    addDeleteAndAddRow(categoryAttributes);

    return categoryAttributes;
}

QTreeWidgetItem *GClassSettings::addCategoryFunctions() {
    // Category 'Functions'
    auto *categoryAttributes = addCategoryHeader("Functions");

    // Attach buttons 'X' and '+' row
    addDeleteAndAddRow(categoryAttributes);

    return categoryAttributes;
}

QTreeWidgetItem *GClassSettings::addCategoryRelations() {
    // Category 'Relations'
    auto *categoryAttributes = addCategoryHeader("Relations TODO");

    // Attach buttons 'X' and '+' row
    addDeleteAndAddRow(categoryAttributes);

    return categoryAttributes;
}