#include "gui/paintwidget.h"
#include <QDebug>
#include <math.h>

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget(parent), currShapeCode(Shape::Line), shape(NULL),pathShape(NULL), perm(false), pixmap(NULL), startX(0), startY(0), goalX(0), goalY(0), startR(0), goalR(0),myRRT(NULL)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    myRRT = NULL;

    shapeList = new QList<Shape*>();
    pathShapeList = new QList<Shape*>();
    currStartShape = NULL;
    currGoalShape = NULL;

    paintTimer = new QTimer(this);
    connect(paintTimer, SIGNAL(timeout()), this, SLOT(update()));

    paintTimer->start(100);
}

void PaintWidget::paintEvent(QPaintEvent *event)
{

//    qDebug() << "PaintWidget->shapeList size: " << shapeList->size();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, size().width(), size().height());

    if(pixmap != NULL){
        painter.drawPixmap(0, 0, this->width(), this->height(), *pixmap);
    }


//    我们在paintEvent()函数中使用了foreach遍历了这个List，绘制出历史图形
    foreach(Shape* shape, *shapeList) {
        if(shape){
            shape->paint(painter);
        }
    }

//    if(shape) {
//        shape->paint(painter);
//    }

    foreach(Shape* pathShape, *pathShapeList) {
        if(pathShape){
            pathShape->paint(painter);
        }
    }

//    if(pathShape) {
//        pathShape->paint(painter);
//    }

}

//首先我们需要按下鼠标，确定直线的第一个点, 所以在mousePressEvent里面，我们让shape保存下start点
void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    switch(currShapeCode)
    {
        case Shape::Line:
        {
            pathShape = NULL;
            shape = new Line(Shape::Line);
            ((Line*)shape)->setColor(Qt::black);
            break;
        }
        case Shape::Rect:
        {
            pathShape = NULL;
            shape = new Rect(Shape::Rect);
            break;
        }
        case Shape::Circle:
        {
            pathShape = NULL;
            shape = new Circle(Shape::Circle);
            break;
        }
        case Shape::StartCircle:
        {
            pathShape = NULL;
            shape = new Circle(Shape::StartCircle);
            // Remove old start shape, and replace it with the new one!
            if(currStartShape != NULL){
                shapeList->removeAt(shapeList->indexOf(currStartShape));
            }
            currStartShape = shape;

            startX = event->x();
            startY = event->y();
            break;
        }
        case Shape::GoalCircle:
        {
            pathShape = NULL;
            shape = new Circle(Shape::GoalCircle);
            if(currGoalShape != NULL){
                shapeList->removeAt(shapeList->indexOf(currGoalShape));
            }
            currGoalShape = shape;

            goalX = event->x();
            goalY = event->y();
            break;
        }
    }

    if(shape != NULL) {
        perm = false;
        //为了保存我们曾经画下的图形，我们使用了一个List。每次按下鼠标时，都会把图形存入这个List
        shapeList->append(shape);
        shape->setStart(event->pos());
        shape->setEnd(event->pos());
    }

    if(pathShape != NULL){
        perm = false;
        pathShapeList->append(pathShape);
        pathShape->setStart(event->pos());
        pathShape->setEnd(event->pos());
    }


}

//然后在鼠标按下的状态下移动鼠标，此时，直线就会发生变化，实际上是直线的终止点在随着鼠标移动，
//所以在mouseMoveEvent中我们让shape保存下end点，然后调用update()函数，
//这个函数会自动调用paintEvent()函数，显示出我们绘制的内容
void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(shape && !perm) {
        shape->setEnd(event->pos());
        update();
    }

    if(pathShape && !perm) {
        pathShape->setEnd(event->pos());
        update();
    }
}

//最后，当鼠标松开时，图形绘制完毕，我们将一个标志位置为true，此时说明这个图形绘制完毕
void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    perm = true;
    if(currShapeCode == Shape::StartCircle)
    {
        startR = (int)(sqrt((double)pow((double)(startX-event->x()),2) + pow((double)(startY-event->y()),2)));
        //qDebug() << "startX: " << startX << ",startEndX:" << event->x() << ",startY:" << startY << ",startEndY:" << event->y() << ",startR" << startR;
    }
    else if(currShapeCode == Shape::GoalCircle)
    {
        goalR = 10 + (int)(sqrt((double)pow((double)(goalX-event->x()),2) + pow((double)(goalY-event->y()),2)));
        //qDebug() << "goalR: " << goalR;
    }
}


QPixmap* PaintWidget::getPixmap()
{
    return this->pixmap;
}

void PaintWidget::setPixmap(QPixmap* pixmap)
{
    this->pixmap = pixmap;
}

QList<Shape*>* PaintWidget::getShapeList()
{
    return shapeList;
}


QList<Shape*>* PaintWidget::getPathShapeList()
{
    return pathShapeList;
}


Shape* PaintWidget::getCurrStartShape()
{
    return currStartShape;
}

Shape* PaintWidget::getCurrGoalShape()
{
    return currGoalShape;
}

void PaintWidget::startSimulationSlot()
{
    myRRT = new MyRRT(this);
    switch(this->type)
    {
    case MyRRT::RRT:
        myRRT->setType(MyRRT::RRT);
        break;
    case MyRRT::RRT_plus:
        myRRT->setType(MyRRT::RRT_plus);
        break;
    case MyRRT::RRT_star:
        myRRT->setType(MyRRT::RRT_star);
        break;
    default:
        ;
    }
    myRRT->setStart_x(startX);
    myRRT->setStart_y(startY);
    myRRT->setStart_r(startR);
    //qDebug() << "start: " << startX << ", " << startY;
    myRRT->setGoal_x(goalX);
    myRRT->setGoal_y(goalY);
    myRRT->setGoal_r(goalR);
    myRRT->setIsRun(true);
    myRRT->setIsStop(false);

   // qDebug() << "goal: " << goalX << ", " << goalY;
    myRRT->setPriority(QThread::HighestPriority);
    myRRT->start();




}


void PaintWidget::stopSimulationSlot()
{
    if(myRRT->isFinished())
    {
        emit currentIteration(myRRT->getIteration());
        emit currentNodeCount(myRRT->getNodeCount());
        emit currentPathLength(myRRT->getPathLengthFromGoal());
    }
}


void PaintWidget::resetSimulationSlot()
{
    if(myRRT != NULL){
        myRRT->setIsRun(false);
        myRRT->setIsStop(true);

        if(!myRRT->isRunning())
        {
            pathShapeList->clear();
        }
    }


}


void PaintWidget::rrtTextChangedSlot(QString name)
{
    QStringList plainningAlgorithm;
    plainningAlgorithm<< "RRT" << "RRT+" << "RRT*";


    // maps "RRT" "RRT+" "RRT*" to 0 1 2 repectively */
    switch(plainningAlgorithm.indexOf(name))
    {
    case 0:
        this->setType(MyRRT::RRT);
        break;
    case 1:
        this->setType(MyRRT::RRT_plus);
        break;
    case 2:
        this->setType(MyRRT::RRT_star);
        break;

    }
    qDebug() << "rrtChanged " << name;
}

void PaintWidget::stepChangedSlot(QString step)
{
    QStringList planningLength;
    planningLength << "4"<< "5"<< "8" <<"10"<<"15"<<"20"<<"25"<<"99999";
    switch(planningLength.indexOf(step))
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    }
}

