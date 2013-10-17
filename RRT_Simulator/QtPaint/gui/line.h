#ifndef LINE_H
#define LINE_H

#include "shape.h"
#include <QColor>


class Line : public Shape
{
public:
    Line(Shape::Code code);

    void paint(QPainter &painter);
    void setColor(Qt::GlobalColor color);

private:
    Qt::GlobalColor color;
};

#endif // LINE_H
