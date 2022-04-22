
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>
#include "GClassSettings.h"
#include <QLineEdit>
#include <QDebug>
#include <QString>
#include <QRadioButton>
#include <QCheckBox>
#include "GClassifier.h"
#include "UMLInterface.h"
#include "UMLClass.h"
#include "ClassDiagram.h"


#define SAVE_BUTTON_STYLE "QPushButton { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"\
                              "stop:0 #6BD96A, stop:0.05 #2BA94A, stop:0.95 #0B892A, stop:1 #06690F); font-size: 30px; border-radius: 3px;}" \
                              "QPushButton:hover { background-color: #25A545;}"\
                              "QPushButton:pressed { background-color: #057525;}"
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

void GClassSettings::hideContent() {
    // Hide all categories
    for(int i = 0; i < tree->topLevelItemCount(); i++){
        tree->topLevelItem(i)->setHidden(true);

        // todo starts at attributes, remove only blank???
        if( i > 1) {
            for (int k = 1; k < tree->topLevelItem(i)->childCount(); k++) {
                tree->topLevelItem(i)->removeChild(tree->topLevelItem(i)->child(k));
                k--; // todo only for removing all children, number of them decreases -> also index
            }
        }
    }
}

void GClassSettings::loadContent(GClassifier *gClassifier){
    //todo add attributes(e.g) rows to categories, when needed

    // Currently selected GClassifier
    selectedGClassifier = gClassifier;

    categories.at(0)->setHidden(false); // Save button panel

    for(int i = 1; i < categories.size(); ++i) {
        bool keepHidden = false;
        QTreeWidgetItem *category = categories.at(i);
        QString categoryName = tree->itemWidget(category, 0)->findChild<QLabel*>()->text();

        // LOADING model data -> view
        if(categoryName.toStdString().find("General") != std::string::npos) {
            tree->itemWidget(categories.at(i)->child(0),0)->findChild<QLineEdit*>()->setText(QString::fromStdString(gClassifier->umlClassifier->name()));
//            tree->itemWidget(categories.at(i)->child(1),0)->findChild<QCheckBox*>()->setChecked(gClassifier->umlClassifier->isAbstract());
        }
        if(categoryName.toStdString().find("Attributes") != std::string::npos){
            UMLClass *cls;
            if((cls = dynamic_cast<UMLClass*>(selectedGClassifier->umlClassifier))) {
                for(auto a : cls->attributes()) {
                    if(dynamic_cast<UMLOperation*>(a) == nullptr) // Is not operation
                        addRow((QWidget*)category, QString::fromStdString(a->type()->name()), QString::fromStdString(a->name()));
                }
            }
            else
                keepHidden = true;
        }
        else if(categoryName.toStdString().find("Functions") != std::string::npos){
            UMLClass *cls;
            if((cls = dynamic_cast<UMLClass*>(selectedGClassifier->umlClassifier))) {
                for(auto a : cls->attributes()) {
                    if(dynamic_cast<UMLOperation*>(a)) // Is operation
                        addRow((QWidget*)category, QString::fromStdString(a->type()->name()), QString::fromStdString(a->name()));
                }
            }
            UMLInterface *inf;
            if((inf = dynamic_cast<UMLInterface*>(selectedGClassifier->umlClassifier))) {
                for(auto a : inf->operations()) {
                    if(dynamic_cast<UMLOperation*>(a)) // Is operation
                        addRow((QWidget*)category, QString::fromStdString(a->type()->name()), QString::fromStdString(a->name()));
                }
            }
        }
        categories.at(i)->setHidden(keepHidden);
    }
}

void GClassSettings::saveContent(){
    if(selectedGClassifier == nullptr)
        return;

    for(int i = 1; i < categories.size(); ++i) {
        QTreeWidgetItem *category = categories.at(i);
        QString categoryName = tree->itemWidget(category, 0)->findChild<QLabel*>()->text();

        if(categoryName.toStdString().find("General") != std::string::npos) {
            // Title
            selectedGClassifier->umlClassifier->setName(tree->itemWidget(category->child(0),0)->findChild<QLineEdit *>()->text().toStdString());
        }
        if(categoryName.toStdString().find("Attributes") != std::string::npos) {
            UMLClass *cls;
            if((cls = dynamic_cast<UMLClass*>(selectedGClassifier->umlClassifier))) {
                // [0] attributes() is the [1] child in a category
                // Save attributes to UML
                int j = 0;
                for(int a = 1; a < category->childCount(); a++){
                    // Assign view values from categories to UML
                    while(j < cls->attributes().size()){
                        // Iterate while Attribute is not operation
                        if(dynamic_cast<UMLOperation*>(cls->attributes()[j]) == nullptr){ // Is not operation
                            cls->getAttributeAtPosition(j)->setType(new UMLClassifier(tree->itemWidget(category->child(a),0)->findChild<QLineEdit*>()->text().toStdString()));
                            cls->getAttributeAtPosition(j)->setName(tree->itemWidget(category->child(a),0)->findChildren<QLineEdit*>()[1]->text().toStdString());
                            j++;
                            break;
                        }
                        j++;
                    }
                    // If the attribute is new, add it to UML
                    if(j >= cls->attributes().size()){
                        std::string name = tree->itemWidget(category->child(a),0)->findChildren<QLineEdit*>()[1]->text().toStdString();
                        UMLClassifier *type = new UMLClassifier(tree->itemWidget(category->child(a),0)->findChild<QLineEdit*>()->text().toStdString());
                        cls->addAttribute(new UMLAttribute(name, type));
                    }
                }
            }
        }
        else if(categoryName.toStdString().find("Functions") != std::string::npos){
            UMLClass *cls;
            if((cls = dynamic_cast<UMLClass*>(selectedGClassifier->umlClassifier))) { // TODO "similar" as class attrinutes saving
                int j = 0;
                for(int a = 1; a < category->childCount(); a++){
                    // Assign view values from categories to UML
                    while(j < cls->attributes().size()){
                        if(dynamic_cast<UMLOperation*>(cls->attributes()[j])){ // Is operation
                            cls->getAttributeAtPosition(j)->setType(new UMLClassifier(tree->itemWidget(category->child(a),0)->findChild<QLineEdit*>()->text().toStdString()));
                            cls->getAttributeAtPosition(j)->setName(tree->itemWidget(category->child(a),0)->findChildren<QLineEdit*>()[1]->text().toStdString());
                            j++;
                            break;
                        }
                        j++;
                    }
                    // If the operation is new, add it to UML
                    if(j >= cls->attributes().size()){
                        std::string name = tree->itemWidget(category->child(a),0)->findChildren<QLineEdit*>()[1]->text().toStdString();
                        UMLClassifier *type = new UMLClassifier(tree->itemWidget(category->child(a),0)->findChild<QLineEdit*>()->text().toStdString());
                        cls->addAttribute(new UMLOperation(name, type));
                    }
                }
            }
            UMLInterface *inf;
            if((inf = dynamic_cast<UMLInterface*>(selectedGClassifier->umlClassifier))) {
                for(int a = 1; a < category->childCount(); a++){
                    int j = a-1;
                    if(j < inf->operations().size()){
                        // Assign view values from categories to UML
                        inf->getOperation(inf->operations()[j]->name())->setType(new UMLClassifier(tree->itemWidget(category->child(a),0)->findChild<QLineEdit*>()->text().toStdString()));
                        inf->getOperation(inf->operations()[j]->name())->setName(tree->itemWidget(category->child(a),0)->findChildren<QLineEdit*>()[1]->text().toStdString());
                    }
                    else{
                        // If the operation is new, add it to UML
                        std::string name = tree->itemWidget(category->child(a),0)->findChildren<QLineEdit*>()[1]->text().toStdString();
                        UMLClassifier *type = new UMLClassifier(tree->itemWidget(category->child(a),0)->findChild<QLineEdit*>()->text().toStdString());
                        inf->addOperation(new UMLOperation(name, type));
                    }
                }
            }
        }
    }
    // notify GClassifier
    connect(this, SIGNAL(contentSaved()), selectedGClassifier, SLOT(contentSaved()));
    emit contentSaved();
}


void GClassSettings::deleteRow(){
    auto list = tree->selectedItems();
    if(list.empty() || selectedGClassifier == nullptr)
        return;
    QTreeWidgetItem *item = tree->selectedItems().at(0);

    // Delete non-top item, which is not the first row with buttons
    if(item != nullptr && item->parent() != nullptr && item->parent()->indexOfChild(item) >= 1) {
        QTreeWidgetItem *category = item->parent();
        QString categoryName = tree->itemWidget(category, 0)->findChild<QLabel*>()->text();

        if(categoryName.toStdString().find("Attributes") != std::string::npos ||
           categoryName.toStdString().find("Functions") != std::string::npos) {
            UMLClass *cls;
            if((cls = dynamic_cast<UMLClass*>(selectedGClassifier->umlClassifier))) {
                cls->removeAttribute(cls->getAttribute(tree->itemWidget(category->child(category->indexOfChild(item)),0)->findChildren<QLineEdit*>()[1]->text().toStdString()));
            }
        }
        if(categoryName.toStdString().find("Functions") != std::string::npos) {
            UMLInterface *inf;
            if((inf = dynamic_cast<UMLInterface*>(selectedGClassifier->umlClassifier))) {
                inf->removeOperation(inf->getOperation(tree->itemWidget(category->child(category->indexOfChild(item)),0)->findChildren<QLineEdit*>()[1]->text().toStdString()));
            }
        }

        item->parent()->removeChild(item);
    }
}

void GClassSettings::selectionChanged(){
    // todo gets first, but must exist..
    QTreeWidgetItem *item = tree->selectedItems().at(0);
    if(item->parent() != nullptr) {
        // Color all to default
        for(int i = 0; i < tree->topLevelItemCount(); ++i) {
            QTreeWidgetItem *topLvl = tree->topLevelItem(i);
            for (int r = 0; r < topLvl->childCount(); ++r) {
                tree->itemWidget(topLvl->child(r), 0)->setStyleSheet("");
            }
        }
        // Color selected non first row
        if(item->parent()->indexOfChild(item) != 0 && tree->indexOfTopLevelItem(item->parent()) > 1)
            tree->itemWidget(item, 0)->setStyleSheet(SELECTED_ROW_STYLE);
    }
}

void GClassSettings::addRow(QWidget *obj, const QString &type, const QString &name) {
    auto *category = (QTreeWidgetItem*)obj;
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
    if(!type.isNull())
        lineEditType->setText(type);
    lineEditType->setContentsMargins(0,0,0,0);
    lineEditType->setMaximumWidth(100);
    rowLayout->addWidget(lineEditType);

    lineEditContent->setPlaceholderText("name");
    if(!name.isNull())
        lineEditContent->setText(name);
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

QTreeWidgetItem *GClassSettings::addCategoryHeader(const QString name){
    auto *category = new QTreeWidgetItem();
    tree->addTopLevelItem(category);
    // Category Frame-Layout Content
    auto *headerLineFrame = new QFrame();
    QBoxLayout* headerLineLayout = new QHBoxLayout(headerLineFrame);

    auto *label = new QLabel("▸    "+name);
    label->setAlignment(Qt::AlignLeft);
    label->setContentsMargins(10,2,0,2);
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
    rowLayout->setContentsMargins(9,9,9,5);

    auto qLabelTitle = new QLabel("Title");
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
    auto *categoryAttributes = addCategoryHeader("Relations");

    // Attach buttons 'X' and '+' row
    addDeleteAndAddRow(categoryAttributes);

    return categoryAttributes;
}