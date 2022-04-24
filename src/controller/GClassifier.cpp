//
// Created by darbix on 21.4.22.
//

#include <GraphicsScene.h>
#include <QBrush>
#include <QDebug>
#include <QLabel>
#include <QtMath>

#include "GClassifier.h"
#include "mainwindow.h"
#include "UMLClassifier.h"
#include "UMLInterface.h"

#define ROW_HEIGHT 25
#define MIN_WIDTH 125
#define ATTRIB_PREFIX "─ "
#define FUNC_PREFIX "+ "

GClassifier::GClassifier(std::string name, qreal x, qreal y, qreal width, qreal height, ClassDiagram *classDiagram, bool isInterface, QGraphicsItem *parent) :
        QGraphicsRectItem(x, y, width, height, parent), QObject(), classDiagram{classDiagram}, isInterface{isInterface}, width{width}, height{height}{

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setBrush(QBrush(Qt::white));

    if(isInterface) {
        umlClassifier = ClassDiagram::createClassifier(name, ClassDiagram::INTERFACE);
//        // todo DEMO DATA SEED
//        if(dynamic_cast<UMLInterface*>(umlClassifier)) {
//            dynamic_cast<UMLInterface *>(umlClassifier)->addOperation(new UMLOperation("Itodo_oper", new UMLClassifier("void")));
//        }
    }
    else {
        umlClassifier = ClassDiagram::createClassifier(name, ClassDiagram::CLASS);
//        // todo DEMO DATA SEED
//        if(dynamic_cast<UMLClass*>(umlClassifier)) {
//            dynamic_cast<UMLClass *>(umlClassifier)->addAttribute(new UMLAttribute("todo_attrib", new UMLClassifier("int")));
//            dynamic_cast<UMLClass *>(umlClassifier)->addAttribute(new UMLOperation("Ctodo_oper", new UMLClassifier("void")));
//        }
    }
    classDiagram->addClassifier(umlClassifier);


    titleRect = new QGraphicsRectItem(x,y, width, ROW_HEIGHT*2, this);
    title = new GText(QString::fromStdString(umlClassifier->name()), titleRect->x(), titleRect->y(), titleRect);
    classificType = new GText(isInterface? "<interface>": "<class>", titleRect->x(), titleRect->y()+title->sceneBoundingRect().height(), titleRect);

    connect(this, SIGNAL(gTextChanged()), title, SLOT(onTextChanged()));
    connect(this, SIGNAL(gTextChanged()), classificType, SLOT(onTextChanged()));
    connect(this, SIGNAL(centerText()), title, SLOT(centerText()));
    connect(this, SIGNAL(centerText()), classificType, SLOT(centerText()));

    if(!isInterface)
        attribRect = new QGraphicsRectItem(x,y+2*ROW_HEIGHT, width, ROW_HEIGHT, this);

//    emit centerText();
    contentSaved();
}
GClassifier::~GClassifier(){
}

void GClassifier::contentSaved(){
    title->setPlainText(QString::fromStdString(umlClassifier->name()));

    for(auto gAttr: gTextAttributes)
        delete gAttr;
    gTextAttributes.clear();

    loadAttributes();

//    emit gTextChanged();                // Notify text so it can resize rectangle back
    resizeRectangles();
    emit centerText();                  // Title texts centering
    emit gClassifierContentChanged();   // Notify GClassDiagram about UML changes
}

void GClassifier::resizeRectangles(){
    qreal w = title->boundingRect().width();

    for(auto a: gTextAttributes){
        w = fmax(w, a->boundingRect().width());
    }

    width = w >= MIN_WIDTH? w: MIN_WIDTH;
    int clsCountHeight = 0;
    int infCountHeight = 0;

    UMLClass *cls;
    if(cls = dynamic_cast<UMLClass*>(umlClassifier)) {
        clsCountHeight += cls->attributes().size() * ROW_HEIGHT;
        attribRect->setRect(0, ROW_HEIGHT*2, width, attribRect->childItems().size()*ROW_HEIGHT);
    }
    UMLInterface *inf;
    if(inf = dynamic_cast<UMLInterface*>(umlClassifier))
        infCountHeight += inf->operations().size() * ROW_HEIGHT;

    height = ROW_HEIGHT*2 + clsCountHeight + infCountHeight;
    setRect(0, 0, width, height);
    titleRect->setRect(0,0,width, ROW_HEIGHT*2);
}

void GClassifier::contentDeleted(){
    contentSaved();
    emit gClassifierContentChanged();
}

QVariant GClassifier::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change == QGraphicsItem::ItemSelectedChange){
        if (value == true) {
            setBrush(QBrush(QColor("#99DCFF")));//  Qt::gray));
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

void GClassifier::loadAttributes(){
    UMLClass *cls = dynamic_cast<UMLClass *>(umlClassifier);
    if(cls) {
        int index = 0;
        // Class Attributes
        for (auto a: cls->attributes()) {
            if (dynamic_cast<UMLOperation *>(a) == nullptr) {
                auto attrib = new GText(ATTRIB_PREFIX+ QString::fromStdString(a->name()) + QString::fromStdString(":  "+a->type()->name()),
                                        boundingRect().x(),ROW_HEIGHT * 2 + (index++ * ROW_HEIGHT) + boundingRect().x(), attribRect);
                connect(this, SIGNAL(gTextChanged()), attrib, SLOT(onTextChanged()));
                gTextAttributes.push_back(attrib);
            }
        }
        // Class operations
        for (auto op: cls->attributes()) {
            if(dynamic_cast<UMLOperation *>(op)) {
                auto attrib = new GText(FUNC_PREFIX + QString::fromStdString(op->name()) + QString::fromStdString("():  "+op->type()->name()),
                                        boundingRect().x(),ROW_HEIGHT * 2 + (index++ * ROW_HEIGHT) + boundingRect().x(), this);
                connect(this, SIGNAL(gTextChanged()), attrib, SLOT(onTextChanged()));
                gTextAttributes.push_back(attrib);
            }
        }
    }
    UMLInterface *inf = dynamic_cast<UMLInterface *>(umlClassifier);
    if(inf) {
        int index = 0;
        // Interface operations
        for (auto op: inf->operations()) {
            if (dynamic_cast<UMLOperation *>(op)) {
                auto attrib = new GText(FUNC_PREFIX+ QString::fromStdString(op->name()) + QString::fromStdString("():  "+op->type()->name()),
                                        boundingRect().x(), ROW_HEIGHT * 2 + (index++ * ROW_HEIGHT) + boundingRect().x(), this);
                connect(this, SIGNAL(gTextChanged()), attrib, SLOT(onTextChanged()));
                gTextAttributes.push_back(attrib);
            }
        }
    }
}