//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_GCLASIFIER_H
#define DIAGRAMEDITOR_GCLASIFIER_H


#include <QGraphicsRectItem>
#include "GRelation.h"
#include "UMLClassifier.h"
#include "GText.h"
#include "ClassDiagram.h"

class GClassifier: public QObject, public QGraphicsRectItem{
Q_OBJECT
public:
    GClassifier(std::string name, qreal x, qreal y, qreal width, qreal height, ClassDiagram *classDiagram, bool isInterface = false, QGraphicsItem *parent = nullptr);
    GClassifier(UMLClassifier *model, qreal x, qreal y, qreal width, qreal height, ClassDiagram *classDiagram, QGraphicsItem *parent = nullptr);
    ~GClassifier();
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    UMLClassifier *umlClassifier;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void gClassifierPositionChanged();
    void gClassifierSelectionChanged();
    qreal gTextChanged();
    void centerText();
    void gClassifierContentChanged();
    void gClassifierDeleted();
private:
    GText *title;
    GText *classificType;
    std::vector<GText*> gTextAttributes;
    std::vector<GRelation*> gRelations;
    bool isInterface;
    ClassDiagram *classDiagram;
    void loadAttributes();
    QGraphicsRectItem *attribRect;
    QGraphicsRectItem *titleRect;
    void resizeRectangles();
    qreal width;
    qreal height;
    qreal x;
    qreal y;
    void keyReleaseEvent(QKeyEvent *event);
private slots:
    void contentSaved();
    void contentDeleted();
    void onUpdateUndefTypes();

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif //DIAGRAMEDITOR_GCLASIFIER_H
