//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_GCLASIFIER_H
#define DIAGRAMEDITOR_GCLASIFIER_H


#include <QGraphicsRectItem>
#include "GRelation.h"
#include "GClassSettings.h"
//#include "ClassSettings.h"


class GClassifier: public QObject, public QGraphicsRectItem{
Q_OBJECT
public:
    GClassifier(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);
    ~GClassifier();
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
signals:
    void gClassifierPositionChanged();

private:
    QGraphicsTextItem *title;
    GClassSettings *gClassSettings;
private slots:
    void contentSaved();
};

#endif //DIAGRAMEDITOR_GCLASIFIER_H
