#ifndef FIELD_ITEM_H
#define FIELD_ITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsPathItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

#include "user_interface/field_related/motion_simulation_tools/field_vectors.h"

using namespace MyVector;

class Arrow;

class FieldItem : public QGraphicsPathItem
{
public:
    enum { Type = UserType + 15 };
    enum FieldItemType { ROBOT_ITEM_TYPE, BALL_ITEM_TYPE, DRAW_ITEM_TYPE };

    FieldItem(FieldItemType ItemType, QMenu *contextMenu,
              QGraphicsItem *parent = 0);

    FieldItemType ItemType() const
    {
        return my_item_type;
    }
    QPolygonF Polygon() const
    {
        return my_polygon;
    }
    QPixmap Image() const;
    int type() const
    {
        return Type;
    }

    //对象可信度
    double location_possibility;
    MyVector::Vector2d<double> oldpos;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  //  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    FieldItemType my_item_type;
    QPolygonF my_polygon;
    QMenu *my_context_menu;
};


#endif // FIELD_ITEM_H
