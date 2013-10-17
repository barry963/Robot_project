#include <QtGui/QApplication>
#include "gui/mainwindow.h"
#include "gui/paintwidget.h"
#include "rrt/miscmethods.h"
#include "rrt/mytree.h"
#include "rrt/mynode.h"
#include <QDebug>
#include <QTime>


#define PI 3.14159265


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle(QObject::tr("Simulator"));
    w.resize(800, 600);
    w.show();

    // Test code


    MiscMethods* mm = new MiscMethods();
//    for(int i=0; i<10; i++)
//    {
//        qDebug() << mm->random();
//    }

//    MyTree* myTree = new MyTree();
//    MyNode* n1 = new MyNode(100,0);
//    MyNode* n2 = new MyNode(150, 0);
//    MyNode* n3 = new MyNode(50, 0);
//    MyNode* n4 = new MyNode(30, 0);
//    MyNode* n5 = new MyNode(200, 0);
//    myTree->addNode(n1, NULL);
//    myTree->addNode(n2, n1);
//    myTree->addNode(n3, n1);
//    myTree->addNode(n4, n1);
//    myTree->addNode(n5, n1);

//    MyNode* randomNode = new MyNode(500,500);
//    QList<MyNode*>* nearestNodes = new QList<MyNode*>();
//    nearestNodes = mm->getNearestNodes(myTree, randomNode, 9999);

//    foreach(MyNode* n, *nearestNodes){
//        qDebug() << n->getX() << "," << n->getY();
//    }


    // Test for rectangle and circle collision
    PaintWidget* p = w.getPaintWidget();


    Circle* c = new Circle(Shape::Circle);
    c->setStart(89,238);
    c->setEnd(89,238+49);
    Rect* rect = new Rect(Shape::Rect);
    rect->setStart(-3,210);
    rect->setEnd(-3+528, 210+136);
//    rect->setAngle(180-30);
    rect->setAngle(180-0.838823*180/PI);



//    p->getPathShapeList()->append(rect);

//    rect = new Rect(Shape::Rect);
//    rect->setStart(-3,210);
//    rect->setEnd(-3+528, 210+136);
//    rect->setAngle(0);
//    p->getPathShapeList()->append(rect);

//    p->getPathShapeList()->append(c);

    //qDebug() << mm->circleAngleRectCollide2(c, rect, 0.838823) << "-----------";
    //qDebug() << mm->circleRectCollide(c, rect);
    //qDebug() << mm->circleAngleRectCollide(c, rect, 0.838823);

//    qDebug() << mm->circleAngleRectCollide(c, rect, 1*PI/180);

//    Circle* c2 = new Circle(Shape::Circle);
//    c2->setStart(1,1);  // Center
//    c2->setEnd(2,2);
//    qDebug() << mm->circleCircleCollide(c, c2);

    return app.exec();
}
