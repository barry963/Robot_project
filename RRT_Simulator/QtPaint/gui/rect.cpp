#include "gui/rect.h"

Rect::Rect(Code code)
{
    this->code = code;
    angle = 0;
}

void Rect::paint(QPainter &painter)
{
//    painter.drawRect(start.x(), start.y(), end.x() - start.x(), end.y() - start.y());

    painter.translate((start.x()+end.x())/2, (start.y()+end.y())/2);
    // Rotates the coordinate system the given angle clockwise.
    painter.rotate(angle);      // In degree!
    painter.translate(-(start.x()+end.x())/2, -(start.y()+end.y())/2);

    painter.setBrush( Qt::NoBrush );
    painter.setPen( Qt::yellow );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(start.x(), start.y(), end.x() - start.x(), end.y() - start.y(), Qt::yellow);

    painter.resetTransform();
}

void Rect::setColor(Qt::GlobalColor color)
{
    this->color = color;
}
