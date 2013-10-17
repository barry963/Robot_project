#include "field_item.h"

#include <QtGui>


//! [0]
FieldItem::FieldItem(FieldItemType item_type, QMenu *context_menu,QGraphicsItem *parent)
    : QGraphicsPathItem(parent)
{
    my_item_type = item_type;
    my_context_menu = context_menu;
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setCursor(Qt::PointingHandCursor);
}

QPixmap FieldItem::Image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(my_polygon);
    return pixmap;
}

void FieldItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    if (my_context_menu)
    {
        my_context_menu->exec(event->screenPos());
    }
}


//! [6]
QVariant FieldItem::itemChange(GraphicsItemChange change,
                               const QVariant &value)
{
    QList<QGraphicsView *> items;
    if (change == QGraphicsItem::ItemPositionChange && scene() )
    {
        // value is the new position.
        QPointF newPos = value.toPointF();
        QRectF rect = scene()->sceneRect();
        if (!rect.contains(newPos))
        {
            // Keep the item inside the scene rect.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

//! [6]
