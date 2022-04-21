//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_GCLASIFIER_H
#define DIAGRAMEDITOR_GCLASIFIER_H


#include <QGraphicsRectItem>
#include "GRelation.h"
#include "UMLClassifier.h"

class GClassifier: public QObject, public QGraphicsRectItem{
Q_OBJECT
public:
    GClassifier(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);
    ~GClassifier();
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    UMLClassifier *umlClassifier;
signals:
    void gClassifierPositionChanged();
    void gClassifierSelectionChanged();
private:
    QGraphicsTextItem *title;
private slots:
    void contentSaved();
};

#endif //DIAGRAMEDITOR_GCLASIFIER_H
