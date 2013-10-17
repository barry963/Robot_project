#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape
{
public:
//    enum Type {
//        Normal,
//        Start,
//        Goal
//    };


//    Circle(Circle::Type t);
    Circle(Shape::Code code);

//    void setType(Circle::Type t);
//    Circle::Type getType();
    void paint(QPainter &painter);
    void setColor(Qt::GlobalColor color);

//private:
//    Circle::Type type;

private:
    Qt::GlobalColor color;
};

#endif // CIRCLE_H
