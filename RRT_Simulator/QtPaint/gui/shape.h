#ifndef SHAPE_H
#define SHAPE_H

#include <QtGui>

class Shape
{
public:

    enum Code {
        Line,
        Rect,
        Circle,
        StartCircle,
        GoalCircle
    };

    Shape();

    void setStart(QPoint s)
    {
        start = s;
    }

    void setStart(int x, int y)
    {
        start = QPoint(x, y);
    }

    void setEnd(QPoint e)
    {
        end = e;
    }

    void setEnd(int x, int y)
    {
        end = QPoint(x, y);
    }

    QPoint startPoint()
    {
        return start;
    }

    QPoint endPoint()
    {
        return end;
    }

    Shape::Code getCode()
    {
        return code;
    }

    void setCode(Shape::Code code)
    {
        this->code = code;
    }


//    paint()������Shape���һ�����麯�������඼����ʵ����������������������������ࣺLine��Rect
    void virtual paint(QPainter & painter) = 0;

protected:
    QPoint start;
    QPoint end;
    Shape::Code code;


};

#endif // SHAPE_H
