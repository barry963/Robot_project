#include "miscmethods.h"


#include <QTime>
#include <QPair>
#include <limits>
#include <math.h>

#define PI 3.14159265

#define SAFE_FACTOR 2

//#define __POINT__
//#define __CIRCLE__
#define __ELLIPSE__



using namespace std;

MiscMethods::MiscMethods()
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    blackRgb = QColor(0, 0, 0).rgb();
    redRgb = QColor(255, 0, 0).rgb();
}


MyNode* MiscMethods::getRandomState(QImage* image, QList<Shape*>* shapeList, int startR, QList<Shape*>* pathShapeList, MyTree* tree)
{
    int x = (int)(random() * image->width());
    int y = (int)(random() * image->height());
//    qDebug() << "getRandomState: image: " << image->width() << ", " << image->height();
//    qDebug() << "getRandomState: x,y: " << x << ", " << y;


    QRgb rgb = image->pixel(x, y);
    MyNode* randNode = new MyNode(x, y);

    // TODO: Check from a single point to an ellipse!!!

#ifdef __POINT__

    while(rgb == blackRgb)
    {
        int x = (int)(random() * image->width());
        int y = (int)(random() * image->height());
        rgb = image->pixel(x, y);
    }
#endif
#ifdef __CIRCLE__

    Shape* randomCircle = NULL;
    bool collide = false;
    bool loop = true;

    while(loop)
    {
        x = (int)(random() * image->width());
        y = (int)(random() * image->height());
        randomCircle = new Circle(Shape::StartCircle);
        randomCircle->setStart(QPoint(x, y));   // Circle center
        randomCircle->setEnd(QPoint(x+startR/sqrt(2), y+startR/sqrt(2)));   // Circle fringe

        collide = false;

        foreach(Shape* s, *shapeList)
        {
            switch(s->getCode())
            {
                case Shape::StartCircle:
                {
                    break;
                }
                case Shape::GoalCircle:
                {
                    break;
                }
                case Shape::Line:
                {
                    break;
                }
                case Shape::Circle:
                {
                    if(circleCircleCollide((Circle*)randomCircle, (Circle*)s)){
                        qDebug() << "random node collision";
                        collide = true;
                    }
                    break;
                }
                case Shape::Rect:
                {
                    break;
                }
            }

            if(collide)
            {
                break;
            }
        }

        if(!collide){
            loop = false;
        }
    }

#endif
#ifdef __ELLIPSE__



    // Currently, I use bigger circle to simulate ellipse
    Shape* randomCircle = NULL;
    bool collide = false;
    bool loop = true;
    int safeDistR = SAFE_FACTOR * (startR/sqrt(2));      // Safe distance

    while(loop)
    {
        x = (int)(random() * image->width());
        y = (int)(random() * image->height());
        randomCircle = new Circle(Shape::StartCircle);
        randomCircle->setStart(QPoint(x, y));   // Circle center
        randomCircle->setEnd(QPoint(x+safeDistR, y+safeDistR));   // Circle fringe

        randNode = new MyNode(x, y);
        // Find the nearest node to newly generated random node
        MyNode* nearestNode = getNearestNode(tree, randNode);
        // Calculate the rotation angle for the surround rectangle
        int nearestX = nearestNode->getX();
        int nearestY = nearestNode->getY();

        double angle = atan2(y-nearestY, x-nearestX);
        int rectWidth = (int)(2*safeDistR);
        int rectHeight = (int)(2*startR);
        int rectCenterX = (int)(x+nearestX)/2;
        int rectCenterY = (int)(y+nearestY)/2;
        Shape* ellipseRect = new Rect(Shape::Rect);

        ellipseRect->setStart(rectCenterX-0.5*rectWidth, rectCenterY-0.5*rectHeight);
        ellipseRect->setEnd(rectCenterX+0.5*rectWidth, rectCenterY+0.5*rectHeight);
        ((Rect*)ellipseRect)->setAngle(angle*180/PI); // CW (clockwise)

//        pathShapeList->append(ellipseRect);

        collide = false;

        foreach(Shape* s, *shapeList)
        {
            switch(s->getCode())
            {
                case Shape::StartCircle:
                {
                    break;
                }
                case Shape::GoalCircle:
                {
                    break;
                }
                case Shape::Line:
                {
                    break;
                }
                case Shape::Circle:
                {
//                    if(circleCircleCollide((Circle*)randomCircle, (Circle*)s)){
                    if(circleAngleRectCollide((Circle*)s, (Rect*)ellipseRect, PI-angle)){
                        qDebug() << "random node collision";
                        collide = true;
                    }
                    break;
                }
                case Shape::Rect:
                {
                    break;
                }
            }

            if(collide)
            {
                break;
            }
        }

        if(!collide){
            loop = false;
        }
    }

#endif

    randNode = new MyNode(x, y);
    ((Circle*)randomCircle)->setColor(Qt::green);
//    pathShapeList->append(randomCircle);
    return randNode;
}


double MiscMethods::random()
{
    return (double)((double)qrand()/RAND_MAX);
}

MyNode* MiscMethods::getNearestNode(MyTree* tree, MyNode* randNode)
{
    int x = randNode->getX();
    int y = randNode->getY();
    double d = 0;
    double min_ = numeric_limits<double>::max();
    MyNode* nearestNode = NULL;
    int vx = -1;
    int vy = -1;
    MyNode* n;

//    qDebug() << "getNearestNode: tree->getNodeCount(): " << tree->getNodeCount();
    for(int i=0; i<tree->getNodeCount(); i++){
        n = tree->getNode(i);
        vx = n->getX();
        vy = n->getY();
        d = sqrt((double)((vx - x) * (vx - x)) + ((vy - y) * (vy - y)));

        if(d < min_){
            min_ = d;
            nearestNode = n;
        }
    }

    return nearestNode;
}


bool MiscMethods::isValidExpansion(QImage *image, MyNode* nearestNode, MyNode* randomNode, double maxDist, QList<Shape*>* shapeList, int startR, QList<Shape*>* pathShapeList)
{
    if(nearestNode == NULL){
        qDebug() << "nearestNode == NULL";
        return false;
    }

    int nx = nearestNode->getX();
    int ny = nearestNode->getY();
    int rx = randomNode->getX();
    int ry = randomNode->getY();

    // Overlap
    if(nx==rx && ny==ry){
        return false;
    }


//    QImage image(paintWidget->size(), QImage::Format_ARGB32);
//    QPainter painter(&image);
//    paintWidget->render(&painter);
//    painter.end();

    double d;
    QRgb rgb;

//    qDebug() << "isValidExpansion?: nearestNode: (" << nx << "," << ny << " .randomNode:(" << rx << "," << ry;

    // Check if there is any obstacle in the distance between nearestNode and randomNode
    // TODO: Check from a single line to an area!!!
#ifdef __POINT__

    if (nx > rx && ny > ry) {
        for (int x = rx; x <= nx; x++) {
            for (int y = ry; y <= ny; y++) {

                d = sqrt((double)(nx - x) * (nx - x) + (ny - y) * (ny - y));

                if (d < maxDist) {
                    if (((x - nx) / (nx - rx)) == ((y - ny) / (ny - ry))) {
                        rgb = image->pixel(x, y);
                        if (rgb == blackRgb || rgb == redRgb) {
                            return false;
                        }
                    }
                }
            }
        }
    } else if (nx > rx && ny < ry) {
        for (int x = rx; x <= nx; x++) {
            for (int y = ry; y >= ny; y--) {

                d = sqrt((double)(nx - x) * (nx - x) + (ny - y) * (ny - y));
                if (d < maxDist) {
                    if (((x - nx) / (nx - rx)) == ((y - ny) / (ny - ry))) {
                        rgb = image->pixel(x, y);
                        if (rgb == blackRgb || rgb == redRgb) {
                            return false;
                        }
                    }
                }
            }
        }
    } else if (nx < rx && ny > ry) {
        for (int x = rx; x >= nx; x--) {
            for (int y = ry; y <= ny; y++) {

                d = sqrt((double)(nx - x) * (nx - x) + (ny - y) * (ny - y));

                if (d < maxDist) {
                    if (((x - nx) / (nx - rx)) == ((y - ny) / (ny - ry))) {
                        rgb = image->pixel(x, y);
                        if (rgb == blackRgb || rgb == redRgb) {
                            return false;
                        }
                    }
                }
            }
        }
    } else if (nx < rx && ny < ry) {
        for (int x = rx; x >= nx; x--) {
            for (int y = ry; y >= ny; y--) {

                d = sqrt((double)(nx - x) * (nx - x) + (ny - y) * (ny - y));
                if (d < maxDist) {
                    if (((x - nx) / (nx - rx)) == ((y - ny) / (ny - ry))) {
                        rgb = image->pixel(x, y);
                        if (rgb == blackRgb || rgb == redRgb) {
                            return false;
                        }
                    }
                }
            }
        }
    } else if (nx == rx && ny > ry) {
        for (int y = ry; y <= ny; y++) {

            d = sqrt((double)(ny - y) * (ny - y));

            if (d < maxDist) {
                rgb = image->pixel(nx, y);
                if (rgb == blackRgb || rgb == redRgb) {
                    return false;
                }
            }
        }
    } else if (nx == rx && ny < ry) {
        for (int y = ry; y >= ny; y--) {

            d = sqrt((double)(ny - y) * (ny - y));

            if (d < maxDist) {
                rgb = image->pixel(nx, y);
                if (rgb == blackRgb || rgb == redRgb) {
                    return false;
                }
            }
        }
    } else if (nx > rx && ny == ry) {
        for (int x = rx; x <= nx; x++) {

            d = sqrt((double)(nx - x) * (nx - x));
            if (d < maxDist) {
                rgb = image->pixel(x, ny);
                if (rgb == blackRgb || rgb == redRgb) {
                    return false;
                }
            }
        }
    } else if (nx < rx && ny == ry) {
        for (int x = rx; x >= nx; x--) {
            d = sqrt((double)(nx - x) * (nx - x));

            if (d < maxDist) {
                rgb = image->pixel(x, ny);
                if (rgb == blackRgb || rgb == redRgb) {
                    return false;
                }
            }
        }
    }

#endif
//#if defined __CIRCLE__ || defined __ELLIPSE__
//#if 1
#ifdef __CIRCLE__

    qDebug() << "=========================";

    // Nearest node center x, y
    int ncx = nearestNode->getX();
    int ncy = nearestNode->getY();
    // Random node center x, y
    int rcx = randomNode->getX();
    int rcy = randomNode->getY();

    double angle = atan2(rcy-ncy, rcx-ncx);
    int rectWidth = (int)(sqrt(pow(ncx-rcx,2) + pow(ncy-rcy,2)));
    int rectHeight = (int)(2*startR);
    int rectCenterX = (int)(rcx+ncx)/2;
    int rectCenterY = (int)(rcy+ncy)/2;
    Shape* sweepRect = new Rect(Shape::Rect);
//TODO: set position for sweepRect!!!
    sweepRect->setStart(rectCenterX-0.5*rectWidth, rectCenterY-0.5*rectHeight);
    sweepRect->setEnd(rectCenterX+0.5*rectWidth, rectCenterY+0.5*rectHeight);
    ((Rect*)sweepRect)->setAngle(angle*180/PI); // CW (clockwise)

//    ((Rect*)pathShapeList)->setColor(Qt::yellow);
//    pathShapeList->append(sweepRect);
    // TODO: rotate the rectanle in rect.cpp!!!
//    ((Rect*)pathShapeList)->setColor(Qt::green);
    bool collide = false;

    foreach(Shape* s, *shapeList)
    {
        switch(s->getCode())
        {
            case Shape::StartCircle:
            {
                break;
            }
            case Shape::GoalCircle:
            {
                break;
            }
            case Shape::Line:
            {
                break;
            }
            case Shape::Circle:
            {
//                qDebug() << "check";
                // CCW (Our collision detection algorithm is based on rotation CCW, while Qt rotate based on CW)
                if(circleAngleRectCollide((Circle*)s, (Rect*)sweepRect, PI-angle)){
                    qDebug() << "path collision-----------------";
                    collide = true;
                    return false;
                }
                break;
            }
            case Shape::Rect:
            {
                break;
            }
        }

        if(collide)
        {
            break;
        }
    }



#endif
#ifdef __ELLIPSE__

    qDebug() << "=========================";

    int safeDistR = SAFE_FACTOR * (startR/sqrt(2));      // Safe distance

    // Nearest node center x, y
    int ncx = nearestNode->getX();
    int ncy = nearestNode->getY();
    // Random node center x, y
    int rcx = randomNode->getX();
    int rcy = randomNode->getY();

    double angle = atan2(rcy-ncy, rcx-ncx);
    int rectWidth = (int)(sqrt(pow(ncx-rcx,2) + pow(ncy-rcy,2)));
    int rectHeight = (int)(2*safeDistR);     // Here we use safeDistR to replace startR
    int rectCenterX = (int)(rcx+ncx)/2;
    int rectCenterY = (int)(rcy+ncy)/2;
    Shape* sweepRect = new Rect(Shape::Rect);
//TODO: set position for sweepRect!!!
    sweepRect->setStart(rectCenterX-0.5*rectWidth, rectCenterY-0.5*rectHeight);
    sweepRect->setEnd(rectCenterX+0.5*rectWidth, rectCenterY+0.5*rectHeight);
    ((Rect*)sweepRect)->setAngle(angle*180/PI); // CW (clockwise)

//    ((Rect*)pathShapeList)->setColor(Qt::yellow);
//    pathShapeList->append(sweepRect);
    // TODO: rotate the rectanle in rect.cpp!!!
//    ((Rect*)pathShapeList)->setColor(Qt::green);
    bool collide = false;

    foreach(Shape* s, *shapeList)
    {
        switch(s->getCode())
        {
            case Shape::StartCircle:
            {
                break;
            }
            case Shape::GoalCircle:
            {
                break;
            }
            case Shape::Line:
            {
                break;
            }
            case Shape::Circle:
            {
//                qDebug() << "check";
                // CCW (Our collision detection algorithm is based on rotation CCW, while Qt rotate based on CW)
                if(circleAngleRectCollide((Circle*)s, (Rect*)sweepRect, PI-angle)){
                    qDebug() << "path collision-----------------";
                    collide = true;
                    return false;
                }
                break;
            }
            case Shape::Rect:
            {
                break;
            }
        }

        if(collide)
        {
            break;
        }
    }

#endif





    return true;
}


MyNode* MiscMethods::goTowardsNode(MyNode* nearestNode, MyNode* randomNode, double maxDist)
{
    int nx = nearestNode->getX();
    int ny = nearestNode->getY();
    int rx = randomNode->getX();
    int ry = randomNode->getY();

//    qDebug() << "MiscMethods::goTowardsNode -- " << nx << "," << ny << "|" << rx << "," << ry;

    // Create a new node based on the random node
    MyNode* newNode = new MyNode(rx, ry);

    double d = distanceBetween(nearestNode, randomNode);
//    qDebug() << "distance: " << d;

    if (d <= maxDist) {
       newNode->setPathLengthFromRoot(nearestNode->getPathLengthFromRoot() + d);
//       qDebug() << "newNode: " << newNode->getX() << "," << newNode->getY();
       return newNode;
    }

    newNode->setPathLengthFromRoot(nearestNode->getPathLengthFromRoot() + maxDist);

    double distFromLine = 0;
    double distFromNearestNode = 0;

    int num = 0;
    double den = 0;

    if (nx > rx && ny > ry) {
       for (int x = rx; x <= nx; x++) {
           for (int y = ry; y <= ny; y++) {

               num = abs((x - nx) * (ny - ry) - (y - ny) * (nx - rx));
               den = sqrt((double)(nx - rx) * (nx - rx) + (ny - ry) * (ny - ry));

               distFromLine = num / den;
               distFromNearestNode = sqrt((double)(x - nx) * (x - nx) + (y - ny) * (y - ny));

               if (distFromLine <= 2 && distFromNearestNode <= maxDist && distFromNearestNode > maxDist - 2) {
                   newNode->setCoordinate(x, y);
                   return newNode;
               }
           }
       }
    } else if (nx > rx && ny < ry) {
       for (int x = rx; x <= nx; x++) {
           for (int y = ry; y >= ny; y--) {

               num = abs((x - nx) * (ny - ry) - (y - ny) * (nx - rx));
               den = sqrt((double)(nx - rx) * (nx - rx) + (ny - ry) * (ny - ry));

               distFromLine = num / den;
               distFromNearestNode = sqrt((double)(x - nx) * (x - nx) + (y - ny) * (y - ny));

               if (distFromLine <= 2 && distFromNearestNode <= maxDist && distFromNearestNode > maxDist - 2) {
                   newNode->setCoordinate(x, y);
                   return newNode;
               }
           }
       }
    } else if (nx < rx && ny > ry) {
       for (int x = rx; x >= nx; x--) {
           for (int y = ry; y <= ny; y++) {

               num = abs((x - nx) * (ny - ry) - (y - ny) * (nx - rx));
               den = sqrt((double)(nx - rx) * (nx - rx) + (ny - ry) * (ny - ry));

               distFromLine = num / den;
               distFromNearestNode = sqrt((double)(x - nx) * (x - nx) + (y - ny) * (y - ny));

               if (distFromLine <= 2 && distFromNearestNode <= maxDist && distFromNearestNode > maxDist - 2) {
                   newNode->setCoordinate(x, y);
                   return newNode;
               }
           }
       }
    } else if (nx < rx && ny < ry) {
       for (int x = rx; x >= nx; x--) {
           for (int y = ry; y >= ny; y--) {

               num = abs((x - nx) * (ny - ry) - (y - ny) * (nx - rx));
               den = sqrt((double)(nx - rx) * (nx - rx) + (ny - ry) * (ny - ry));

               distFromLine = num / den;
               distFromNearestNode = sqrt((double)(x - nx) * (x - nx) + (y - ny) * (y - ny));

               if (distFromLine <= 2 && distFromNearestNode <= maxDist && distFromNearestNode > maxDist - 2) {
                   newNode->setCoordinate(x, y);
                   return newNode;
               }
           }
       }
    } else if (nx == rx && ny > ry) {
       for (int y = ry; y <= ny; y++) {

           num = 0;
           den = sqrt((double)(ny - ry) * (ny - ry));

           distFromNearestNode = sqrt((double)(y - ny) * (y - ny));

           if (distFromNearestNode <= maxDist && distFromNearestNode > maxDist - 2) {
               newNode->setCoordinate(nx, y);
               return newNode;
           }
       }
    } else if (nx == rx && ny < ry) {
       for (int y = ry; y >= ny; y--) {

           num = 0;
           den = sqrt((double)(ny - ry) * (ny - ry));

           distFromNearestNode = sqrt((double)(y - ny) * (y - ny));

           if (distFromNearestNode <= maxDist && distFromNearestNode > maxDist - 2) {
               newNode->setCoordinate(nx, y);
               return newNode;
           }
       }
    } else if (nx > rx && ny == ry) {
       for (int x = rx; x <= nx; x++) {

           den = sqrt((double)(nx - rx) * (nx - rx));

           distFromNearestNode = sqrt((double)(x - nx) * (x - nx));

           if (distFromNearestNode <= maxDist && distFromNearestNode > maxDist - 2) {
               newNode->setCoordinate(x, ny);
               return newNode;
           }
       }
    } else if (nx < rx && ny == ry) {
       for (int x = rx; x >= nx; x--) {
           den = sqrt((double)(nx - rx) * (nx - rx));

           distFromNearestNode = sqrt((double)(x - nx) * (x - nx));

           if (distFromNearestNode <= maxDist && distFromNearestNode > maxDist - 2) {
               newNode->setCoordinate(x, ny);
               return newNode;
           }
       }
    }
    return NULL;
}



double MiscMethods::distanceBetween(MyNode* n1, MyNode* n2)
{
    int x1 = n1->getX();
    int y1 = n1->getY();
    int x2 = n2->getX();
    int y2 = n2->getY();
    return sqrt((double)pow((double)(x1 - x2),2) + pow((double)(y1 - y2),2));
}


void MiscMethods::drawLineSegment(QList<Shape*>* shapeList, MyNode* nearestNode, MyNode* newNode)
{
    int vx = nearestNode->getX();
    int vy = nearestNode->getY();
    int rx = newNode->getX();
    int ry = newNode->getY();

    Shape* shape = new Line(Shape::Line);
    shape->setStart(QPoint(vx, vy));
    shape->setEnd(QPoint(rx, ry));
    shapeList->append(shape);


//    qDebug() << "MiscMethods::drawLineSegment -- shapeList size: " << shapeList->size() << vx << "," << vy << "|" << rx << "," << ry;

//    QPainter painter;
//    painter.setBrush( Qt::NoBrush );
//    painter.setPen( Qt::blue );
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.drawLine(vx, vy, rx, ry);

}


bool MiscMethods::goalReached(MyNode* newNode, int goal_x, int goal_y, int goal_r)
{
    int x = newNode->getX();
    int y = newNode->getY();

    double d = sqrt((double)(goal_x - x) * (goal_x - x) + (goal_y - y) * (goal_y - y));
    if (d <= goal_r) {
        return true;
    } else {
        return false;
    }
}

QList<MyNode*>* MiscMethods::getPath(MyNode* start, MyNode* end, MyTree* tree)
{
    QList<MyNode*>* path = new QList<MyNode*>();
    path->push_back(end);
    MyNode* n = end;
    while (true) {
        n = tree->getParent(n);
        path->push_back(n);
        if (n == start) {
            return path;
        }
    }
}


void MiscMethods::highLightPath(QList<Shape*>* shapeList, QList<MyNode*>* path, double edgeLength)
{
    int x1, y1, x2, y2;
    int dotRadius = 4;


    double nodesInPath = path->size();
    double r_v = 200/(nodesInPath-1);
    double g_v = 150/(nodesInPath-1);

    switch ((int)edgeLength) {
        case 4:
            dotRadius = 4;
            break;
        case 5:
            dotRadius = 4;
            break;
        case 8:
            dotRadius = 6;
            break;
        case 10:
            dotRadius = 8;
            break;
        case 15:
            dotRadius = 10;
            break;
        case 20:
            dotRadius = 10;
            break;
        case 25:
            dotRadius = 12;
            break;
        case 99999:
            dotRadius = 12;
            break;
    }

    double r = 200;
    double g = 0;
    double b = 0;

    for (int i = 0; i < path->size()-1; i++) {

        r = 200-((double)i*r_v);
        g = 0+((double)i*g_v);
//        QColor color((int)ceil(r), (int)ceil(g), (int)b);
        //color = new Color(200,0,0);

        x1 = (*path)[i]->getX();
        y1 = (*path)[i]->getY();

        x2 = (*path)[i+1]->getX();
        y2 = (*path)[i+1]->getY();

        Shape* shape = new Line(Shape::Line);
        if(i%2 == 0){
            ((Line*)shape)->setColor(Qt::red);
        }
        else{
            ((Line*)shape)->setColor(Qt::blue);
        }

        shape->setStart(QPoint(x1, y1));
        shape->setEnd(QPoint(x2, y2));
        shapeList->append(shape);

//        g2D.setColor(color);
//        g2D.setStroke(new BasicStroke(3));
//        g2D.drawLine(x1, y1, x2, y2);
//        g2D.setColor(color);
    }
}


QList<MyNode*>* MiscMethods::getNearestNodes(MyTree* tree, MyNode* node, double radius)
{
    int x = node->getX();
    int y = node->getY();
    double dist;
    double pathLength;

    QHash<MyNode*, double>* nearestNodes = new QHash<MyNode*, double>();

    for(int i = 0; i < tree->getNodeCount(); ++i){
        dist = distanceBetween(tree->getNode(i),node);
        pathLength = tree->getNode(i)->getPathLengthFromRoot();
        if(dist < radius)
            nearestNodes->insert(tree->getNode(i), pathLength + dist);
    }




    QList< QPair<MyNode*, double>* >* sortedNodes = new QList< QPair<MyNode*, double>* >();
    sortedNodes = sortHashMap(nearestNodes);




    QList<MyNode*>* candidateNodes = new QList<MyNode*>();

    for(int i=0; i<sortedNodes->size(); ++i){
        candidateNodes->append(sortedNodes->at(i)->first);
    }

    return candidateNodes;
}


QList< QPair<MyNode*, double>* >* MiscMethods::sortHashMap(QHash<MyNode*, double>* input)
{
    QHash<MyNode*, double>* tempMap = new QHash<MyNode*, double>();
    QList<MyNode*>* mapKeys = new QList<MyNode*>();
    QList<double>* mapValues = new QList<double>(tempMap->values());
//    QHash<MyNode*, double>* sortedMap = new QHash<MyNode*, double>();   // unordered
    // Use QList with QPair to replace LinkedHashMap structure in Java!
    QList< QPair<MyNode*, double>* >* sortedMap = new QList< QPair<MyNode*, double>* >();

    QMap<double, double>* sortedSet = new QMap<double, double>();   // ordered

    foreach(MyNode* wsState, input->keys())
    {
        double wsStateValue = input->value(wsState);
        tempMap->insert(wsState, wsStateValue);
        mapKeys->append(wsState);
        mapValues->append(wsStateValue);
        sortedSet->insert(wsStateValue, wsStateValue);
    }

//    qDebug() << "sortedSet:";
//    QMapIterator<double, double> i(*sortedSet);
//    while (i.hasNext()) {
//       i.next();
//       qDebug() << i.key() << ": " << i.value();
//    }
//    qDebug() << "=========================";



//    qDebug() << "mapValues:";
//    foreach(double d, *mapValues){
//        qDebug() << d;
//    }
//    qDebug() << "=========================";


    QMapIterator<double, double> sortedSetIter(*sortedSet);
    while(sortedSetIter.hasNext()){
        sortedSetIter.next();
        double curKey = sortedSetIter.key();
        MyNode* sortedSetKey = mapKeys->at(mapValues->indexOf(curKey));
//        qDebug() << "curKey:" << curKey << ",index:" << mapValues->indexOf(curKey) << ",mapKeys:(" << sortedSetKey->getX() << "," << sortedSetKey->getY() << ")";

        QPair<MyNode*, double>* sortedPair = new QPair<MyNode*, double>(sortedSetKey, curKey);
        sortedMap->append(sortedPair);

    }

//    qDebug() << "sortedMap:";
//    for(int i=0; i<sortedMap->size(); ++i){
//        qDebug() << sortedMap->at(i)->first->getX() << "," << sortedMap->at(i)->first->getY() << ": " << sortedMap->at(i)->second;
//    }

//    qDebug() << "=========================";

    return sortedMap;
}


void MiscMethods::updateEdges(QList<Shape*>* shapeList, MyTree* tree, MyNode* start, MyNode* goal)
{
//    g2D.drawImage(map,0,0, jp);
    shapeList->clear();
    highLightPath(shapeList, getPath(start, goal, tree), 0);
}


bool MiscMethods::circleCircleCollide(Circle* s1, Circle* s2)
{
    int x1 = s1->startPoint().x();      // Center(x1,y1)
    int y1 = s1->startPoint().y();
    int x2 = s2->startPoint().x();
    int y2 = s2->startPoint().y();
//    qDebug() << "x1:" << x1 << ",y1:" << y1 << ",s1->endPoint().x():" << s1->endPoint().x() << ",s1->endPoint().y():" << s1->endPoint().y();
//    qDebug() << "pow(x1 - s1->endPoint().x(), 2.0):" << pow(x1 - s1->endPoint().x(), 2.0);
    double r1 = sqrt((double)(pow(x1 - s1->endPoint().x(), 2.0) + pow(y1 - s1->endPoint().y(), 2.0)));
    double r2 = sqrt((double)(pow(x2 - s2->endPoint().x(), 2.0) + pow(y2 - s2->endPoint().y(), 2.0)));

//    qDebug() << "r1:" << r1 << ", r2:" << r2;
//    qDebug() << pow(x1-x2, 2.0) + pow(y1-y2, 2.0);
//    qDebug() << pow(r1+r2, 2.0);
    return (pow(x1-x2, 2.0) + pow(y1-y2, 2.0) <= pow(r1+r2, 2.0));
}

bool MiscMethods::circleRectCollide(Circle *s1, Rect *s2)
{
    int circleX = s1->startPoint().x();
    int circleY = s1->startPoint().y();
    double r = sqrt((double)(pow(circleX - s1->endPoint().x(), 2.0) + pow(circleY - s1->endPoint().y(), 2.0)));

    // Center of rectangle
    double rectX = 0.5*(s2->startPoint().x() + s2->endPoint().x());
    double rectY = 0.5*(s2->startPoint().y() + s2->endPoint().y());
    int rectWidth = abs((ceil)(s2->endPoint().x() - s2->startPoint().x()));
    int rectHeight = abs((ceil)(s2->endPoint().y() - s2->startPoint().y()));

    int distX = abs((int)(circleX - rectX));
    int distY = abs((int)(circleY - rectY));

    if(distX > rectWidth/2.0 + r){return false;}
    if(distY > rectHeight/2.0 + r){return false;}

    if(distX <= rectWidth/2.0) {return true;}
    if(distY <= rectHeight/2.0) {return true;}

    double cornerDist_sq = pow(distX-rectWidth/2.0, 2.0) + pow(distY-rectHeight/2.0, 2.0);

    return (cornerDist_sq <= pow(r, 2.0));
}

// Here is about counter-clockwise rotation! angle in radiant!
bool MiscMethods::circleAngleRectCollide(Circle *s1, Rect *s2, double angle)
{
    // Circle center
    int circleX = s1->startPoint().x();
    int circleY = s1->startPoint().y();
    double r = sqrt((double)(pow(circleX - s1->endPoint().x(), 2.0) + pow(circleY - s1->endPoint().y(), 2.0)));

    qDebug() << "angle: " << angle;
    qDebug() << "circleX:" << circleX << ",circleY:" << circleY << ",r:" << r;

    // Center of rectangle
    double rectCenterX = 0.5*(s2->startPoint().x() + s2->endPoint().x());
    double rectCenterY = 0.5*(s2->startPoint().y() + s2->endPoint().y());

    // Rectangle's left-top point
    double rectX = s2->startPoint().x();
    double rectY = s2->startPoint().y();

    int rectWidth = abs((ceil)(s2->endPoint().x() - s2->startPoint().x()));
    int rectHeight = abs((ceil)(s2->endPoint().y() - s2->startPoint().y()));

    qDebug() << "rectX:" << rectX << ", rectY: " << rectY;
    qDebug() << "rectCenterX:" << rectCenterX << ",rectCenterY:" << rectCenterY << ",rectWidth:" << rectWidth << ",rectHeight:" << rectHeight;

    double cx = cos(angle)*(circleX-rectCenterX) - sin(angle)*(circleY-rectCenterY) + rectCenterX;
    double cy = sin(angle)*(circleX-rectCenterX) + cos(angle)*(circleY-rectCenterY) + rectCenterY;

    qDebug() << "cx:" << cx << ",cy:" << cy;
    double x, y;

    if(cx < rectX){
        x = rectX;
    }
    else if(cx > rectX+rectWidth){
        x = rectX + rectWidth;
    }
    else{
        x = cx;
    }

    if(cy < rectY){
        y = rectY;
    }
    else if(cy > rectY+rectHeight){
        y = rectY + rectHeight;
    }
    else{
        y = cy;
    }

    qDebug() << "x:" << x << ",y:" << y;
    double distance = findDistance(cx, cy, x, y);
    qDebug() << "distance: " << distance << ", r: " << r;

    if(distance <= r){
        qDebug() << "collide true";
        return true;
    }
    else{
        qDebug() << "collide false";
        return false;
    }
}


double MiscMethods::findDistance(double x1, double y1, double x2, double y2)
{

//    qDebug() << sqrt(pow(x1-x2,2.0) + pow(y1-y2,2.0));
    return sqrt(pow(x1-x2,2.0) + pow(y1-y2,2.0));

    // Buggy code, can lose accuracy in C++ !!!
//    double a = abs(x1-x2);
//    double b = abs(y1-y2);
//    qDebug() << "a:" << a << ",b:" << b;
//    double c = sqrt((a*a) + (b*b));
//    return c;
}











// Backup circle-rotate-rect collision detection algorithm
bool MiscMethods::circleAngleRectCollide2(Circle *s1, Rect *s2, double angle)
{
    // Circle center
    int circleX = s1->startPoint().x();
    int circleY = s1->startPoint().y();
    double r = sqrt((double)(pow(circleX - s1->endPoint().x(), 2.0) + pow(circleY - s1->endPoint().y(), 2.0)));


    // Center of rectangle
    double rectCenterX = 0.5*(s2->startPoint().x() + s2->endPoint().x());
    double rectCenterY = 0.5*(s2->startPoint().y() + s2->endPoint().y());

    // Rectangle's left-top point
    double rectX = s2->startPoint().x();
    double rectY = s2->startPoint().y();

    int rectWidth = abs((ceil)(s2->endPoint().x() - s2->startPoint().x()));
    int rectHeight = abs((ceil)(s2->endPoint().y() - s2->startPoint().y()));

    double tx, ty, cx, cy;

    if(angle == 0) { // Higher Efficiency for Rectangles with 0 rotation.
        tx = circleX;
        ty = circleY;

        cx = rectCenterX;
        cy = rectCenterY;
    } else {
        tx = cos(angle)*circleX - sin(angle)*circleY;
        ty = cos(angle)*circleY + sin(angle)*circleX;

        cx = cos(angle)*rectCenterX - sin(angle)*rectCenterY;
        cy = cos(angle)*rectCenterY + sin(angle)*rectCenterX;
    }

    return testRectangleToPoint(rectWidth, rectHeight, angle, rectCenterX, rectCenterY, circleX, circleY) ||
            testCircleToSegment(tx, ty, angle, cx-rectWidth/2, cy+rectHeight/2, cx+rectWidth/2, cy+rectHeight/2) ||
            testCircleToSegment(tx, ty, angle, cx+rectWidth/2, cy+rectHeight/2, cx+rectWidth/2, cy-rectHeight/2) ||
            testCircleToSegment(tx, ty, angle, cx+rectWidth/2, cy-rectHeight/2, cx-rectWidth/2, cy-rectHeight/2) ||
            testCircleToSegment(tx, ty, angle, cx-rectWidth/2, cy-rectHeight/2, cx-rectWidth/2, cy+rectHeight/2);
}


/** Rectangle To Point. */
bool MiscMethods::testRectangleToPoint(double rectWidth, double rectHeight, double rectRotation, double rectCenterX, double rectCenterY, double pointX, double pointY) {
    if(rectRotation == 0)   // Higher Efficiency for Rectangles with 0 rotation.
        return abs(rectCenterX-pointX) < rectWidth/2 && abs(rectCenterY-pointY) < rectHeight/2;

    double tx = cos(rectRotation)*pointX - sin(rectRotation)*pointY;
    double ty = cos(rectRotation)*pointY + sin(rectRotation)*pointX;

    double cx = cos(rectRotation)*rectCenterX - sin(rectRotation)*rectCenterY;
    double cy = cos(rectRotation)*rectCenterY + sin(rectRotation)*rectCenterX;

    return abs(cx-tx) < rectWidth/2 && abs(cy-ty) < rectHeight/2;
}

/** Circle To Segment. */
bool MiscMethods::testCircleToSegment(double circleCenterX, double circleCenterY, double circleRadius, double lineAX, double lineAY, double lineBX, double lineBY) {
    double lineSize = sqrt(pow(lineAX-lineBX, 2) + pow(lineAY-lineBY, 2));
    double distance;

    if (lineSize == 0) {
        distance = sqrt(pow(circleCenterX-lineAX, 2) + pow(circleCenterY-lineAY, 2));
        return distance < circleRadius;
    }

    double u = ((circleCenterX - lineAX) * (lineBX - lineAX) + (circleCenterY - lineAY) * (lineBY - lineAY)) / (lineSize * lineSize);

    if (u < 0) {
        distance = sqrt(pow(circleCenterX-lineAX, 2) + pow(circleCenterY-lineAY, 2));
    } else if (u > 1) {
        distance = sqrt(pow(circleCenterX-lineBX, 2) + pow(circleCenterY-lineBY, 2));
    } else {
        double ix = lineAX + u * (lineBX - lineAX);
        double iy = lineAY + u * (lineBY - lineAY);
        distance = sqrt(pow(circleCenterX-ix, 2) + pow(circleCenterY-iy, 2));
    }

    return distance < circleRadius;
}
