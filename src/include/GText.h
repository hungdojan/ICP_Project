/**
 * @brief Declaration of GText class
 * Resizable Graphic text item
 *
 * @file GText.h
 * @date 07/05/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_GTEXT_H
#define DIAGRAMEDITOR_GTEXT_H

#include <QGraphicsTextItem>
#include <QObject>

/**
 * Resizable text item class
 */
class GText: public QGraphicsTextItem {
Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param text String text
     * @param x X position
     * @param y Y position
     * @param parent Parent objcet
     */
    GText(const QString &text, qreal x, qreal y, QGraphicsItem *parent = nullptr);

private:
    /** Parent graphic object */
    QGraphicsItem *parent;

private slots:
    /**
     * @brief Center text in the parent line
     */
    void centerText();
};

#endif //DIAGRAMEDITOR_GTEXT_H
