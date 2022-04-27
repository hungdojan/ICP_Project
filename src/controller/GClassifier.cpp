//
// Created by darbix on 21.4.22.
//

#include <GraphicsScene.h>
#include <QBrush>
#include <QDebug>
#include <QLabel>
#include <QtMath>
#include <QKeyEvent>

#include "GClassifier.h"
#include "mainwindow.h"
#include "UMLClassifier.h"
#include "UMLInterface.h"
#include "UMLOperation.h"

#define ROW_HEIGHT 25
#define MIN_WIDTH 125
#define SELECTED_COLOR "#99DCFF"
#define UNSELECTED_COLOR "#CFCFDD"

GClassifier::GClassifier(std::string name, qreal x, qreal y, qreal width, qreal height, ClassDiagram *classDiagram, bool isInterface, QGraphicsItem *parent) :
        QGraphicsRectItem(x, y, width, height, parent), QObject(), classDiagram{classDiagram}, isInterface{isInterface}, width{width}, height{height}, x{x}, y{y}{

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setBrush(QBrush(QColor(UNSELECTED_COLOR)));

    if(isInterface)
        umlClassifier = ClassDiagram::createClassifier(name, ClassDiagram::INTERFACE);
    else
        umlClassifier = ClassDiagram::createClassifier(name, ClassDiagram::CLASS);

    classDiagram->addClassifier(umlClassifier);

    titleRect = new QGraphicsRectItem(x,y, width, ROW_HEIGHT*2, this);
    title = new GText(QString::fromStdString(umlClassifier->name()), x, y, titleRect);
    classificType = new GText(isInterface? "<interface>": "<class>", x, y+title->sceneBoundingRect().height(), titleRect);

    connect(this, SIGNAL(gTextChanged()), title, SLOT(onTextChanged()));
    connect(this, SIGNAL(gTextChanged()), classificType, SLOT(onTextChanged()));
    connect(this, SIGNAL(centerText()), title, SLOT(centerText()));
    connect(this, SIGNAL(centerText()), classificType, SLOT(centerText()));

    if(!isInterface)
        attribRect = new QGraphicsRectItem(x,y+2*ROW_HEIGHT, width, ROW_HEIGHT, this);

    contentSaved();

}
GClassifier::~GClassifier(){
}

void GClassifier::onUpdateUndefTypes(){
    contentSaved();
}

void GClassifier::contentSaved(){
    title->setPlainText(QString::fromStdString(umlClassifier->name()));

    for(auto gAttr: gTextAttributes)
        delete gAttr;
    gTextAttributes.clear();

    loadAttributes();

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
        attribRect->setRect(x, y+ROW_HEIGHT*2, width, attribRect->childItems().size()*ROW_HEIGHT);
    }
    UMLInterface *inf;
    if(inf = dynamic_cast<UMLInterface*>(umlClassifier))
        infCountHeight += inf->operations().size() * ROW_HEIGHT;

    height = ROW_HEIGHT*2 + clsCountHeight + infCountHeight;
    setRect(x, y, width, height);
    titleRect->setRect(x, y, width, ROW_HEIGHT*2);
    emit gClassifierPositionChanged();

//    qDebug() << sceneBoundingRect().x() << " " << sceneBoundingRect().y(); todo save as x, y??
}

void GClassifier::contentDeleted(){
    contentSaved();
    emit gClassifierContentChanged();
}

QVariant GClassifier::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change == QGraphicsItem::ItemSelectedChange){
        if (value == true) {
            setBrush(QBrush(QColor(SELECTED_COLOR)));
        }
        else {
            setBrush(QBrush(QColor(UNSELECTED_COLOR)));
        }
        emit gClassifierSelectionChanged();
    }
    if(change == ItemPositionChange && scene()) {
        emit gClassifierPositionChanged();
    }

    return QGraphicsItem::itemChange(change, value);
}

void GClassifier::keyReleaseEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Delete)
        emit gClassifierDeleted();
}

void GClassifier::loadAttributes(){
    UMLClass *cls = dynamic_cast<UMLClass *>(umlClassifier);
    if(cls) {
        int index = 0;
        // Class Attributes
        for (auto a: cls->attributes()) {
            if (dynamic_cast<UMLOperation *>(a) == nullptr) {
                auto attrib = new GText(QString::fromStdString(std::string(*a)),
                                        boundingRect().x(),ROW_HEIGHT * 2 + (index++ * ROW_HEIGHT) + boundingRect().x(), attribRect);
                connect(this, SIGNAL(gTextChanged()), attrib, SLOT(onTextChanged()));
                gTextAttributes.push_back(attrib);
            }
        }
        // Class operations
        for (auto a: cls->attributes()) {
            UMLOperation *op = dynamic_cast<UMLOperation *>(a);
            if(op) {
                auto attrib = new GText(QString::fromStdString(std::string(*op)),
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
        for (auto a: inf->operations()) {
            UMLOperation *op = dynamic_cast<UMLOperation *>(a);
            if(op) {
                auto attrib = new GText(QString::fromStdString(std::string(*op)),
                                        boundingRect().x(), ROW_HEIGHT * 2 + (index++ * ROW_HEIGHT) + boundingRect().x(), this);
                connect(this, SIGNAL(gTextChanged()), attrib, SLOT(onTextChanged()));
                gTextAttributes.push_back(attrib);
            }
        }
    }
}