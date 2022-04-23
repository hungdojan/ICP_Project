//
// Created by darbix on 21.4.22.
//

#include <QBrush>
#include "GClassifier.h"
#include "mainwindow.h"
#include <GraphicsScene.h>
#include "UMLClassifier.h"
#include "UMLInterface.h"
#include <QDebug>
#include <QLabel>

GClassifier::GClassifier(std::string name, qreal x, qreal y, qreal width, qreal height, ClassDiagram *classDiagram, bool isInterface, QGraphicsItem *parent) :
        QGraphicsRectItem(x, y, width, height, parent), QObject(), classDiagram{classDiagram}, isInterface{isInterface}{

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setBrush(QBrush(Qt::white));

    if(isInterface) {
        umlClassifier = ClassDiagram::createClassifier(name, ClassDiagram::INTERFACE);
        // todo DEMO DATA SEED
        if(dynamic_cast<UMLInterface*>(umlClassifier)) {
            dynamic_cast<UMLInterface *>(umlClassifier)->addOperation(new UMLOperation("Itodo_oper", new UMLClassifier("void")));
        }
    }
    else {
        umlClassifier = ClassDiagram::createClassifier(name, ClassDiagram::CLASS);
        // todo DEMO DATA SEED
        if(dynamic_cast<UMLClass*>(umlClassifier)) {
            dynamic_cast<UMLClass *>(umlClassifier)->addAttribute(new UMLAttribute("todo_attrib", new UMLClassifier("int")));
            dynamic_cast<UMLClass *>(umlClassifier)->addAttribute(new UMLOperation("Ctodo_oper", new UMLClassifier("void")));
        }
    }
    classDiagram->addClassifier(umlClassifier);


    auto *titleRect = new QGraphicsRectItem(x,y, width, 50, this);
    title = new GText(QString::fromStdString(umlClassifier->name()), titleRect->x(), titleRect->y(), titleRect);
    classificType = new GText(isInterface? "<interface>": "<class>", titleRect->x(), titleRect->y()+title->sceneBoundingRect().height(), titleRect);

    connect(this, SIGNAL(gTextChanged()), title, SLOT(onTextChanged()));
    connect(this, SIGNAL(gTextChanged()), classificType, SLOT(onTextChanged()));
    connect(this, SIGNAL(centerText()), title, SLOT(centerText()));
    connect(this, SIGNAL(centerText()), classificType, SLOT(centerText()));

    emit centerText();
}
GClassifier::~GClassifier(){
}

void GClassifier::contentSaved(){
    title->setPlainText(QString::fromStdString(umlClassifier->name()));

// TODO
//    if(gTextAttributes.empty()) {
//        UMLClass *cls = dynamic_cast<UMLClass *>(umlClassifier);
//        for (int i = 0; i < cls->attributes().size(); i++) {
//            gTextAttributes.push_back(new GText(QString::fromStdString(cls->attributes()[i]->name()), boundingRect().x(), 50 + (i * 25) + boundingRect().x(), this));
//        }
//    }

    emit gTextChanged();
    // After all resizes, texts can be centered
    emit centerText();
}

QVariant GClassifier::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change == QGraphicsItem::ItemSelectedChange){
        if (value == true) {
            setBrush(QBrush(Qt::gray));
        }
        else {
            setBrush(QBrush(Qt::lightGray));
        }
        emit gClassifierSelectionChanged();
    }
    if(change == ItemPositionChange && scene()) {
        emit gClassifierPositionChanged();
    }
    return QGraphicsItem::itemChange(change, value);
}
