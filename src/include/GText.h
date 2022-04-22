//
// Created by darbix on 21.4.22.
//

#ifndef DIAGRAMEDITOR_GTEXT_H
#define DIAGRAMEDITOR_GTEXT_H

#include <QGraphicsTextItem>
#include <QObject>

class GText: public QGraphicsTextItem {
Q_OBJECT
public:
    GText(const QString &text, qreal x, qreal y, QGraphicsItem *parent = nullptr);
//    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
//    void onCenterText();
private:
    QGraphicsItem *parent;
private slots:
    void onTextChanged();
    void centerText();
};

#endif //DIAGRAMEDITOR_GTEXT_H
