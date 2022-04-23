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
    ~GClassifier();
    QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    UMLClassifier *umlClassifier;
    std::vector<GText*> gTextAttributes;
signals:
    void gClassifierPositionChanged();
    void gClassifierSelectionChanged();
    void gTextChanged();
    void centerText();
private:
    GText *title;
    GText *classificType;
    bool isInterface;
    ClassDiagram *classDiagram;
private slots:
    void contentSaved();
};

#endif //DIAGRAMEDITOR_GCLASIFIER_H
