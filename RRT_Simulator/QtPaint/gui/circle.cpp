#include "gui/circle.h"

//Circle::Circle(Type t)
//{
//    this->type = t;
//}

Circle::Circle(Shape::Code code)
{
    this->code = code;
}

void Circle::paint(QPainter &painter)
{
    // Fill ellipse
    switch(this->code)
    {
        case Shape::Circle:
        {
            painter.setBrush( QBrush( QColor(0, 0, 0) ) );
            break;
        }
        case Shape::StartCircle:
        {
            painter.setBrush( QBrush( QColor(0, 255, 0) ) );
            break;
        }
        case Shape::GoalCircle:
        {
            painter.setBrush( QBrush( QColor(255, 0, 0) ) );
            break;
        }
    }


    painter.setPen( Qt::NoPen );

    // Draw ellipse
//    painter.setBrush( Qt::NoBrush );
//    painter.setPen( Qt::black );

    QPointF center = QPointF(start.x(), start.y());
    int radius = sqrt((double)((start.x()-end.x())*(start.x()-end.x()) + (start.y()-end.y())*(start.y()-end.y())));
//    qDebug() << "radius: " << radius;
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawEllipse(center, (qreal)radius, (qreal)radius);
}

//void Circle::setType(Circle::Type t)
//{
//    this->type = t;
//}



//Circle::Type Circle::getType()
//{
//    return this->type;
//}

void Circle::setColor(Qt::GlobalColor color)
{
    this->color = color;
}
