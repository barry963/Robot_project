#include "gui/line.h"

Line::Line(Code code)
{
    this->code = code;
    color = Qt::black;
}

void Line::paint(QPainter &painter)
{
    painter.setBrush( Qt::NoBrush );
    painter.setPen( color );
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawLine(start, end);
}


void Line::setColor(Qt::GlobalColor color)
{
    this->color = color;
}
