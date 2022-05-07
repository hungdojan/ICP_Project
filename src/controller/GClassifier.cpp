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
        QGraphicsRectItem(x, y, width, height, parent), QObject(), classDiagram{classDiagram}, isInterface{isInterface}, width{width}, height{height}, x{0}, y{0}{

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
//    x = 0;
//    y = 0;

    titleRect = new QGraphicsRectItem(this->x,this->y, width, ROW_HEIGHT*2, this);
    title = new GText(QString::fromStdString(umlClassifier->name()), this->x, this->y, titleRect);
    classificType = new GText(isInterface? "<interface>": "<class>", this->x, this->y+title->sceneBoundingRect().height(), titleRect);

    connect(this, SIGNAL(gTextChanged()), title, SLOT(onTextChanged()));
    connect(this, SIGNAL(gTextChanged()), classificType, SLOT(onTextChanged()));
    connect(this, SIGNAL(centerText()), title, SLOT(centerText()));
    connect(this, SIGNAL(centerText()), classificType, SLOT(centerText()));

    if(!isInterface)
        attribRect = new QGraphicsRectItem(this->x,this->y+2*ROW_HEIGHT, width, ROW_HEIGHT, this);
    setPos(x, y);
    umlClassifier->x() = x;
    umlClassifier->y() = y;
//    this->x = 0;
//    this->y = 0;

    contentSaved();

}

GClassifier::GClassifier(UMLClassifier *model, qreal x, qreal y, qreal width, qreal height, ClassDiagram *classDiagram, QGraphicsItem *parent) :
        QGraphicsRectItem(x, y, width, height, parent), QObject(), umlClassifier{model}, classDiagram{classDiagram}, isInterface{dynamic_cast<UMLInterface *>(model) != nullptr}, width{width}, height{height}, x{0}, y{0}{

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setBrush(QBrush(QColor(UNSELECTED_COLOR)));

    titleRect = new QGraphicsRectItem(this->x,this->y, width, ROW_HEIGHT*2, this);
    title = new GText(QString::fromStdString(umlClassifier->name()), this->x, this->y, titleRect);
    classificType = new GText(isInterface? "<interface>": "<class>", this->x, this->y+title->sceneBoundingRect().height(), titleRect);

    connect(this, SIGNAL(gTextChanged()), title, SLOT(onTextChanged()));
    connect(this, SIGNAL(gTextChanged()), classificType, SLOT(onTextChanged()));
    connect(this, SIGNAL(centerText()), title, SLOT(centerText()));
    connect(this, SIGNAL(centerText()), classificType, SLOT(centerText()));

    if(!isInterface)
        attribRect = new QGraphicsRectItem(this->x,this->y+2*ROW_HEIGHT, width, ROW_HEIGHT, this);
    // this->x = 0;
    // this->y = 0;

    contentSaved();
    setPos(x, y);
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

    umlClassifier->x() = sceneBoundingRect().x();
    umlClassifier->y() = sceneBoundingRect().y();
}

void GClassifier::contentDeleted(){
    contentSaved();
    emit gClassifierContentChanged();
}
#include "ICommand.h"
#include "CommandBuilder.h"
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
                auto attrib = new GText(QString::fromStdString(std::string(*a)),x,
                                        ROW_HEIGHT * 2 + (index++ * ROW_HEIGHT) + y, attribRect);
                connect(this, SIGNAL(gTextChanged()), attrib, SLOT(onTextChanged()));
                gTextAttributes.push_back(attrib);
            }
        }

        // Class operations
        for (auto a: cls->attributes()) {
            UMLOperation *op = dynamic_cast<UMLOperation *>(a);
            if(op) {
                auto attrib = new GText(QString::fromStdString(std::string(*op)),
                        x,ROW_HEIGHT * 2 + (index++ * ROW_HEIGHT) + y, this);
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
                                        x, ROW_HEIGHT * 2 + (index++ * ROW_HEIGHT) + y, this);
                connect(this, SIGNAL(gTextChanged()), attrib, SLOT(onTextChanged()));
                gTextAttributes.push_back(attrib);
            }
        }
    }
}

void GClassifier::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);
}

void GClassifier::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseMoveEvent(event);
}

void GClassifier::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    class Cmd : public ICommand {
        GClassifier *cls;
        double origX = 0;
        double origY = 0;
        double newX = 0;
        double newY = 0;
    public:
        explicit Cmd(GClassifier *c) : cls{c} {
            origX = cls->umlClassifier->x();
            origY = cls->umlClassifier->y();
            auto pos = cls->pos();
            newX = pos.x();
            newY = pos.y();
        }
        void execute() override {
            cls->setPos(newX, newY);
            cls->umlClassifier->x() = newX;
            cls->umlClassifier->y() = newY;
            emit cls->gClassifierPositionChanged();
        }
        void undo() override {
            cls->setPos(origX, origY);
            cls->umlClassifier->x() = origX;
            cls->umlClassifier->y() = origY;
            emit cls->gClassifierPositionChanged();
            // TODO:
        }
        void redo() override {
            cls->setPos(newX, newY);
            cls->umlClassifier->x() = newX;
            cls->umlClassifier->y() = newY;
            emit cls->gClassifierPositionChanged();
        }
    };
    std::shared_ptr<ICommand> cmd{new Cmd(this)};
    CommandBuilder::get_commander().execute(cmd);
}
