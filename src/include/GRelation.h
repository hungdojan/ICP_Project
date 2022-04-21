#ifndef DIAGRAMEDITOR_GRELATION_H
#define DIAGRAMEDITOR_GRELATION_H

#include <QGraphicsLineItem>

class GRelation : public QObject, public QGraphicsLineItem {
Q_OBJECT
public:
    GRelation(QGraphicsItem *parent, QGraphicsItem *target);
private:
    QGraphicsItem *parent;
    QGraphicsItem *target;
private slots:
    void updatePos();
};

#endif //DIAGRAMEDITOR_GRELATION_H
