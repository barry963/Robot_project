#ifndef  PAINT_BUFFER_H
#define PAINT_BUFFER_H

#include <QLine>
#include <QPoint>
#include <QMutex>
#include <QString>
#include <QPainter>

const int MAXLINECMDS=400;
const int MAXPOINTCMDS=500;
const int MAXTEXTCMDS=100;
const int MAXDEBUGROBOT = 1000;

struct MyText
{
        QPointF pos;
        QString text;
};

struct RobotDebug
{
    QPainterPath body;
    qreal x;
    qreal y;
    qreal z;
};

class PaintCmd
{
public:
        PaintCmd();
        void AddLine(qreal& x0,qreal& y0,qreal& x1,qreal& y1,int color);
        void AddPoint(qreal& x0,qreal& y0);
        void AddText(qreal& x0,qreal& y0,QString s);
        void AddVelo(qreal& x0,qreal& y0,qreal& x1,qreal& y1);
        void AddRobot(qreal x0,qreal y0,qreal z0);

        void Clear();
        void ExecCmds(QPainter * painter);

        int iLineCount;
        int iPointCount;
        int iTextCount;
        int iVeloCount;
        int iRobotCount;
        int iRobotLastCount;

        QLineF		velolines[MAXLINECMDS];
        QLineF		lines[MAXLINECMDS];
        int colorArray[MAXLINECMDS];
        QPointF		points[MAXPOINTCMDS];
        MyText    texts[MAXTEXTCMDS];
        RobotDebug robotdebug[MAXDEBUGROBOT];
};

class PaintCmds
{
public:
        PaintCmds();
        void StrategySwitchCmds();
        void GuiSwitchCmds();
        void AddPoint(qreal& x0,qreal& y0);
        void AddLine(qreal& x0,qreal& y0,qreal& x1,qreal& y1,int color);
        void AddText(qreal& x0,qreal& y0,QString s);
        void AddVelo(qreal& x0,qreal& y0,qreal& x1,qreal& y1);
        void AddRobot(qreal& x0,qreal& y0,qreal& z0);

        void ExecCmds(QPainter * painter);

        void CmdsSwitch(int iType=0);
        QMutex IdleMutex;

        int iStrategy;		//用于策略绘制
        int iGui;					//用于GUI绘制
        int iIdle;				//空闲
        PaintCmd Cmds[3];
};

extern PaintCmds GuiCmd;

#endif
