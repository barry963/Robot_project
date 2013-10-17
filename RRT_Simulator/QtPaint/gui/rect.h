#ifndef RECT_H
#define RECT_H

#include "shape.h"

class Rect : public Shape
{
public:
    Rect(Shape::Code code);

    void paint(QPainter &painter);

    void setColor(Qt::GlobalColor color);

    // Clockwise in degree!!!
    void setAngle(double angle)
    {
        this->angle = angle;
    }


    double angle;

private:
    Qt::GlobalColor color;
};

#endif // RECT_H
