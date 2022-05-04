
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
#include <QComboBox>
#include "GClassifier.h"
#include "UMLInterface.h"
#include "UMLClass.h"
#include "ClassDiagram.h"
#include "UMLRelation.h"

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

GClassSettings::GClassSettings(QTreeWidget *tree,  ClassDiagram *classDiagram): QObject(), classDiagram{classDiagram}{
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
        if(categoryName.toStdString().find(GENERAL) != std::string::npos) {
            tree->itemWidget(categories.at(i)->child(0),0)->findChild<QLineEdit*>()->setText(QString::fromStdString(gClassifier->umlClassifier->name()));
        }
        if(categoryName.toStdString().find(ATTRIBUTES) != std::string::npos){
            UMLClass *cls;
            if((cls = dynamic_cast<UMLClass*>(selectedGClassifier->umlClassifier))) {
                for(auto a : cls->attributes()) {
                    if(dynamic_cast<UMLOperation*>(a) == nullptr) // Is not operation
                        addRow((QWidget*)category, QString::fromStdString(std::string(1,a->visibility())),
                               QString::fromStdString(a->type()->name()), QString::fromStdString(a->name()));
                }
            }
            else
                keepHidden = true;
        }
        else if(categoryName.toStdString().find(FUNCTIONS) != std::string::npos){
            UMLClass *cls;
            if((cls = dynamic_cast<UMLClass*>(selectedGClassifier->umlClassifier))) {
                for(auto a : cls->attributes()) {
                    if(dynamic_cast<UMLOperation*>(a)) // Is operation
                        loadOperation(category, dynamic_cast<UMLOperation*>(a));
                }
            }
            UMLInterface *inf;
            if((inf = dynamic_cast<UMLInterface*>(selectedGClassifier->umlClassifier))) {
                for(auto a : inf->operations()) {
                    if(a) // Is operation
                        loadOperation(category, a);
                }
            }
        }
        else if (categoryName.toStdString().find(RELATIONS) != std::string::npos) {
            int a = 1; // category child item index to set
            for (UMLRelation *r: selectedGClassifier->umlClassifier->relations()) {
                // Set the "relation to: " class to dst/src depending on the direction
                auto inRelationClass = r->dst()->name() == selectedGClassifier->umlClassifier->name()?
                        QString::fromStdString(r->src()->name()): QString::fromStdString(r->dst()->name());

                auto msg1 = r->dst()->name() == selectedGClassifier->umlClassifier->name()? QString::fromStdString(r->dstMsg()): QString::fromStdString(r->srcMsg());
                auto msg2 = r->src()->name() == selectedGClassifier->umlClassifier->name()? QString::fromStdString(r->dstMsg()): QString::fromStdString(r->srcMsg());

                addRelationRow((QWidget*)category, inRelationClass, msg1, msg2);
                a++;
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
        QString categoryName = tree->itemWidget(category, 0)->findChild<QLabel *>()->text();

        if (categoryName.toStdString().find(GENERAL) != std::string::npos) {
            // Title
            std::string name = tree->itemWidget(category->child(0), 0)->findChild<QLineEdit *>()->text().toStdString();
            if(!name.empty() && !classDiagram->getClassifier(name))
                selectedGClassifier->umlClassifier->setName(name);
        }
        if (categoryName.toStdString().find(ATTRIBUTES) != std::string::npos) {
            UMLClass *cls;
            if ((cls = dynamic_cast<UMLClass *>(selectedGClassifier->umlClassifier))) {
                // Remove old attributes
                for(auto a: cls->attributes()) {
                    if(!dynamic_cast<UMLOperation*>(a))
                        cls->removeAttribute(a);
                }
                // Add from settings panel to UML
                for (int a = 1; a < category->childCount(); a++) {
                    std::string name = tree->itemWidget(category->child(a),0)->findChildren<QLineEdit *>()[1]->text().toStdString();
                    std::string typeName = tree->itemWidget(category->child(a),0)->findChild<QLineEdit *>()->text().toStdString();
                    UMLClassifier *type = classDiagram->getClassifier(typeName);
                    if(!type) {
                        type = new UMLClassifier(typeName);
                        classDiagram->addClassifier(type);
                    }

                    if(!name.empty()) {
                        auto attr = UMLClassifier::createAttribute(false, name, type, {});
                        attr->visibility() = tree->itemWidget(category->child(a),0)->findChild<QComboBox*>()->currentText().toStdString()[0];
                        cls->addAttribute(attr);
                    }
                }
            }
        } else if (categoryName.toStdString().find(FUNCTIONS) != std::string::npos) {
            UMLClass *cls;
            if ((cls = dynamic_cast<UMLClass *>(selectedGClassifier->umlClassifier))) { // TODO "similar" as class attributes saving
                // Remove old operations
                for(auto a: cls->attributes()){
                    if(dynamic_cast<UMLOperation*>(a))
                        cls->removeAttribute(a);
                }
                // Add from settings panel to UML
                for (int a = 1; a < category->childCount(); a++) {
                    std::string typeName = tree->itemWidget(category->child(a),0)->findChild<QLineEdit*>()->text().toStdString();
                    std::string name = tree->itemWidget(category->child(a),0)->findChildren<QLineEdit*>()[1]->text().toStdString();
                    if(!typeName.empty() && !name.empty()) {
                        UMLClassifier *type = classDiagram->getClassifier(typeName);
                        if (!type) {
                            type = new UMLClassifier(typeName);
                            classDiagram->addClassifier(type);
                        }

                        if (!name.empty()) {
                            auto attr = dynamic_cast<UMLOperation *>(UMLClassifier::createAttribute(true, name, type,{}));
                            fillOperation(attr, category, a);
                            cls->addAttribute(attr);
                        }
                    }
                }
            }
            UMLInterface *inf;
            if ((inf = dynamic_cast<UMLInterface *>(selectedGClassifier->umlClassifier))) {
                // Remove old operations
                for(auto a: inf->operations())
                    inf->removeOperation(a);
                // Add from settings panel to UML
                for (int a = 1; a < category->childCount(); a++) {
                    std::string name = tree->itemWidget(category->child(a),0)->findChildren<QLineEdit *>()[1]->text().toStdString();
                    std::string typeName = tree->itemWidget(category->child(a),0)->findChild<QLineEdit *>()->text().toStdString();
                    if(!typeName.empty() && !name.empty()) {
                        UMLClassifier *type = classDiagram->getClassifier(typeName);
                        if (!type) {
                            type = new UMLClassifier(typeName);
                            classDiagram->addClassifier(type);
                        }

                        if (!name.empty()){
                            auto attr = dynamic_cast<UMLOperation *>(UMLClassifier::createAttribute(true, name, type,{}));
                            fillOperation(attr, category, a);
                            inf->addOperation(attr);
                        }
                    }
                }
            }
        }
        else if (categoryName.toStdString().find(RELATIONS) != std::string::npos) {
            // remove all? todo
            selectedGClassifier->umlClassifier->clearRelations();

            for (int a = 1; a < category->childCount(); a++) {
                std::string name = tree->itemWidget(category->child(a),0)->findChild<QLineEdit*>()->text().toStdString();
                // If classifier exists and is not already in the relation
                if(classDiagram->getClassifier(name) && !selectedGClassifier->umlClassifier->inRelationWith(classDiagram->getClassifier(name)) &&
                        name != selectedGClassifier->umlClassifier->name()) {
                    selectedGClassifier->umlClassifier->addRelation(classDiagram->getClassifier(name));
                    auto rel = selectedGClassifier->umlClassifier->getRelationWith(classDiagram->getClassifier(name));
                    rel->srcMsg() = tree->itemWidget(category->child(a),0)->findChildren<QComboBox*>()[0]->currentText().toStdString();
                    rel->dstMsg() = tree->itemWidget(category->child(a),0)->findChildren<QComboBox*>()[1]->currentText().toStdString();
//                    if(rel->dstMsg() == INHERITANCE_SYMB) {
//                        rel->setRelationType(UMLRelation::INHERITANCE); //todo
//                    }
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

        if(categoryName.toStdString().find(ATTRIBUTES) != std::string::npos ||
           categoryName.toStdString().find(FUNCTIONS) != std::string::npos) {
            UMLClass *cls;
            if((cls = dynamic_cast<UMLClass*>(selectedGClassifier->umlClassifier))) {
                cls->removeAttribute(cls->getAttribute(tree->itemWidget(category->child(category->indexOfChild(item)),0)->findChildren<QLineEdit*>()[1]->text().toStdString()));
            }
        }
        if(categoryName.toStdString().find(FUNCTIONS) != std::string::npos) {
            UMLInterface *inf;
            if((inf = dynamic_cast<UMLInterface*>(selectedGClassifier->umlClassifier))) {
                inf->removeOperation(inf->getOperation(tree->itemWidget(category->child(category->indexOfChild(item)),0)->findChildren<QLineEdit*>()[1]->text().toStdString()));
            }
        }
        else if(categoryName.toStdString().find(RELATIONS) != std::string::npos) {
            auto name = tree->itemWidget(category->child(category->indexOfChild(item)),0)->findChild<QLineEdit*>()->text().toStdString();
//            auto rel = selectedGClassifier->umlClassifier->getRelationWith(classDiagram->getClassifier(name));
            selectedGClassifier->umlClassifier->removeRelation(classDiagram->getClassifier(name));
        }

        item->parent()->removeChild(item);
    }

    // notify GClassifier
    connect(this, SIGNAL(contentDeleted()), selectedGClassifier, SLOT(contentDeleted()));
    emit contentDeleted();
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

void GClassSettings::addRow(QWidget *obj, const QString &access, const QString &type, const QString &name) {
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

    QString x = ACCESSIBILITIES;
    auto *comboBoxAccess = new QComboBox();
    comboBoxAccess->addItems(QStringList(x.split(",")));
    comboBoxAccess->setFixedWidth(50);
    if(!access.isEmpty())
        comboBoxAccess->setCurrentText(access);
    rowLayout->addWidget(comboBoxAccess);

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

void GClassSettings::addFuncRow(QWidget *obj, const QString &access, const QString &ret, const QString &name, const QString &params) {
    auto *category = (QTreeWidgetItem*)obj;

    // New attribute row
    auto *row = new QTreeWidgetItem();
    category->addChild(row);
    // Row Frame-Layout Content
    auto *rowFrame = new QFrame();
    QBoxLayout *rowLayoutV = new QVBoxLayout(rowFrame);
    rowLayoutV->setContentsMargins(9, 5, 9, 5);
    rowLayoutV->setSpacing(0);

    auto *rowFrame1 = new QFrame();
    auto *rowFrame2 = new QFrame();

    rowLayoutV->addWidget(rowFrame1);
    rowLayoutV->addWidget(rowFrame2);

    QBoxLayout *rowLayoutH1 = new QHBoxLayout(rowFrame1);
    QBoxLayout *rowLayoutH2 = new QHBoxLayout(rowFrame2);
    rowLayoutH1->setContentsMargins(0,0,0,4);
    rowLayoutH2->setContentsMargins(0,0,0,0);

    QString x = ACCESSIBILITIES;
    auto *comboBoxAccess = new QComboBox();
    comboBoxAccess->addItems(QStringList(x.split(",")));
    comboBoxAccess->setFixedWidth(50);
    if(!access.isEmpty())
        comboBoxAccess->setCurrentText(access);
    rowLayoutH1->addWidget(comboBoxAccess);

    auto *lineEditType = new QLineEdit();
    auto *lineEditContent = new QLineEdit();

    lineEditType->setPlaceholderText("type");
    if(!ret.isNull())
        lineEditType->setText(ret);
    lineEditType->setContentsMargins(0,0,0,0);
    lineEditType->setMaximumWidth(100);
    rowLayoutH1->addWidget(lineEditType);

    lineEditContent->setPlaceholderText("name");
    if(!name.isNull())
        lineEditContent->setText(name);
    lineEditContent->setContentsMargins(0,0,0,0);
    rowLayoutH1->addWidget(lineEditContent);

    auto *lineEditParams = new QLineEdit();
    lineEditParams->setPlaceholderText("type1 param1, ...");
    if(!params.isEmpty())
        lineEditParams->setText(params);
    lineEditParams->setContentsMargins(0,0,0,0);
    rowLayoutH2->addWidget(lineEditParams);

    tree->setItemWidget(row, 0, rowFrame);

    category->setExpanded(false); // todo function else overwrites relations category
    category->setExpanded(true);
}

void GClassSettings::addRelationRow(QWidget *obj, const QString &dst, const QString &srcMsg, const QString &dstMsg) {
    auto *category = (QTreeWidgetItem *) obj;

    // New attribute row
    auto *row = new QTreeWidgetItem();
    category->addChild(row);
    // Row Frame-Layout Content
    auto *rowFrame = new QFrame();
    QBoxLayout *rowLayoutV = new QVBoxLayout(rowFrame);
    rowLayoutV->setContentsMargins(10, 3, 9, 7);
    rowLayoutV->setSpacing(0);

    auto *rowFrame1 = new QFrame();
    auto *rowFrame2 = new QFrame();

    rowLayoutV->addWidget(rowFrame1);
    rowLayoutV->addWidget(rowFrame2);

    QBoxLayout *rowLayoutH1 = new QHBoxLayout(rowFrame1);
    QBoxLayout *rowLayoutH2 = new QHBoxLayout(rowFrame2);
    rowLayoutH1->setContentsMargins(0,0,0,4);
    rowLayoutH1->setSpacing(10);
    rowLayoutH2->setContentsMargins(0,0,0,0);
    rowLayoutH2->setSpacing(10);

    auto *lineEditParent = new QLabel();
    lineEditParent->setText("from: " + QString::fromStdString(selectedGClassifier->umlClassifier->name()));
    lineEditParent->setStyleSheet(RELATION_PARENT_STYLE);
    lineEditParent->setFixedWidth(185);
    lineEditParent->setContentsMargins(0,0,0,0);
    rowLayoutH1->addWidget(lineEditParent);

    auto *lineEditTarget = new QLineEdit();
    lineEditTarget->setPlaceholderText("to: ");
    if(!dst.isEmpty())
        lineEditTarget->setText(dst);
    lineEditTarget->setContentsMargins(0,0,0,0);
    rowLayoutH1->addWidget(lineEditTarget);

    QString x = RELATION_TYPES;
    auto *comboBoxParent = new QComboBox();
    comboBoxParent->addItems(QStringList(x.split(",")));
    comboBoxParent->setFixedWidth(185);
    if(!srcMsg.isEmpty())
        comboBoxParent->setCurrentText(srcMsg);
    rowLayoutH2->addWidget(comboBoxParent);

    auto *comboBoxTarget = new QComboBox();
    comboBoxTarget->addItems(QStringList(x.split(",")));
    if(!dstMsg.isEmpty())
        comboBoxTarget->setCurrentText(dstMsg);
    rowLayoutH2->addWidget(comboBoxTarget);

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
    QString categoryName = tree->itemWidget(category, 0)->findChild<QLabel*>()->text();

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
    connect(addButton, SIGNAL(released()), mapper, SLOT(map()));
    mapper->setMapping(addButton, (QWidget*)category);
    if(categoryName.toStdString().find(RELATIONS) != std::string::npos)
        connect(mapper, SIGNAL(mapped(QWidget*)), this, SLOT(addRelationRow(QWidget*)));
    else if(categoryName.toStdString().find(FUNCTIONS) != std::string::npos)
        connect(mapper, SIGNAL(mapped(QWidget*)), this, SLOT(addFuncRow(QWidget*)));
    else
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
    auto *category = addCategoryHeader(GENERAL);

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
    auto *categoryAttributes = addCategoryHeader(ATTRIBUTES);

    // Attach buttons 'X' and '+' row
    addDeleteAndAddRow(categoryAttributes);

    return categoryAttributes;
}

QTreeWidgetItem *GClassSettings::addCategoryFunctions() {
    // Category 'Functions'
    auto *categoryAttributes = addCategoryHeader(FUNCTIONS);

    // Attach buttons 'X' and '+' row
    addDeleteAndAddRow(categoryAttributes);

    return categoryAttributes;
}

QTreeWidgetItem *GClassSettings::addCategoryRelations() {
    // Category 'Relations'
    auto *categoryAttributes = addCategoryHeader(RELATIONS);

    // Attach buttons 'X' and '+' row
    addDeleteAndAddRow(categoryAttributes);

    return categoryAttributes;
}

void GClassSettings::fillOperation(UMLOperation *attr, QTreeWidgetItem *category, int index){
    QString params = tree->itemWidget(category->child(index),0)->findChildren<QLineEdit *>()[2]->text();
    for (auto paramText: QStringList(params.split(","))) {
        if(paramText.isEmpty()) continue;

        paramText = paramText.trimmed();
        auto paramAndType = QStringList(paramText.split(" "));
        if(paramAndType.size() >= 2) {
            auto parType = classDiagram->getClassifier(paramAndType[0].toStdString());
            if (!parType) { // Then create
                parType = new UMLClassifier(paramAndType[0].toStdString());
                classDiagram->addClassifier(parType);
            }
            auto parName = paramAndType[1];
            for(int i = 1; i < paramAndType.size() && parName.isEmpty(); i++)
                parName = paramAndType[i];
            attr->addParameter(new UMLAttribute(parName.toStdString(), parType));
        }
    }
    attr->visibility() = tree->itemWidget(category->child(index),0)->findChild<QComboBox*>()->currentText().toStdString()[0];
}

void GClassSettings::loadOperation(QTreeWidgetItem *category, UMLOperation *a){
    std::string paramsText = "";
    for(auto param: a->parameters())
        paramsText += param->type()->name()+" "+param->name()+",";
    if(paramsText.size() > 0)
        paramsText.erase(paramsText.end()-1);
    addFuncRow((QWidget *) category, QString::fromStdString(std::string(1,a->visibility())),
               QString::fromStdString(a->type()->name()),QString::fromStdString(a->name()), QString::fromStdString(paramsText));
}