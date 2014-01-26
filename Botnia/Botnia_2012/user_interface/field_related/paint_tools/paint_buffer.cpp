#include "paint_buffer.h"

#include "paint_buffer.h"
#include <QDebug>
#include "math.h"

PaintCmds GuiCmd;

//--------------------------------------------------------------------
PaintCmd::PaintCmd()
{
        Clear();
};

void PaintCmd::AddLine(qreal& x0,qreal& y0,qreal& x1,qreal& y1)
{
        int iLine=iLineCount;
        if (iLine<MAXLINECMDS)
        {
                lines[iLine].setLine(x0,y0,x1,y1);
                iLineCount=iLine+1;
        }
};

void PaintCmd::AddVelo(qreal &x0, qreal &y0, qreal &x1, qreal &y1)
{
    int iVeloLine=iVeloCount;
    if (iVeloLine<MAXLINECMDS)
    {
            lines[iVeloLine].setLine(x0,y0,x1,y1);
            iVeloCount=iVeloLine+1;
    }
}

void PaintCmd::AddPoint(qreal& x0,qreal& y0)
{
        int iPoint=iPointCount;
        if (iPoint<MAXPOINTCMDS)
        {
                points[iPoint].setX(x0);
                points[iPoint].setY(y0);
                iPointCount=iPoint+1;
        }
};

void PaintCmd::AddText(qreal& x0,qreal& y0,QString s)
{
        int iText=iTextCount;
        if (iText<MAXTEXTCMDS)
        {
                texts[iText].pos.setX(x0);
                texts[iText].pos.setY(y0);
                texts[iText].text=s;
                iTextCount=iText+1;
        }
};

void PaintCmd::AddRobot(qreal &x0, qreal &y0, qreal &z0)
{
    int iRobot = iRobotCount;
    if (iRobot<MAXDEBUGROBOT)
    {
        if ( fabs ( z0 ) <360 )
        {
            robotdebug[iRobot].body.moveTo ( 90+x0,0+y0 );
            robotdebug[iRobot].body.arcTo ( -90+x0,-90+y0,180,180,0,270 );
            robotdebug[iRobot].body.closeSubpath();
        }
        else
        {
            robotdebug[iRobot].body.addEllipse ( -90+x0,-90+y0,180,180 );
        }
        robotdebug[iRobot].x = x0;
        robotdebug[iRobot].y = y0;
        robotdebug[iRobot].z = z0*180.0/M_PI;

        iRobotCount=iRobot+1;
    }
}

void PaintCmd::ExecCmds(QPainter * painter)
{

    if(iRobotCount)
    {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::black, 2));

        for(int i=0;i<iRobotCount;i++)
        {
            double _orientation = robotdebug[i].z;
            if ( fabs ( _orientation ) <360 )
            {
                painter->rotate ( -45+_orientation );
                painter->drawPath ( robotdebug[i].body );
                painter->rotate ( 45-_orientation );
            }
            else
            {
                painter->drawPath ( robotdebug[i].body );
            }
            qDebug()<<"targetdebug #"<<i<<", ="<<robotdebug[i].x<<","<<robotdebug[i].y<<","<<robotdebug[i].z;
        }
        //painter->scale(1,-1);
        //painter->restore();

     }

        if (iLineCount)
        {
            painter->setPen(QPen(Qt::gray, 10, Qt::DotLine));
            painter->drawLines(lines,iLineCount);
        }
        if (iPointCount)
        {
            painter->drawPoints(points,iPointCount);
        }

        if(iVeloCount)
        {
            painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
            painter->drawLines(velolines,iVeloCount);
        }


        int i;
        painter->setPen(Qt::red);
        painter->save();
        //painter->scale(10,-10);
        for (i=0; i<iTextCount; i++)
        {
                painter->drawText(texts[i].pos,texts[i].text);
        }
        painter->restore();
}

void PaintCmd::Clear()
{
        iLineCount=0;
        iPointCount=0;
        iTextCount=0;
        iVeloCount=0;
        //iRobotCount = 0;
}

//------------------------------------------------------------------
PaintCmds::PaintCmds()
{
        iStrategy=0;
        iGui=1;
        iIdle=2;
}

void PaintCmds::AddLine(qreal& x0,qreal& y0,qreal& x1,qreal& y1)
{
        Cmds[iStrategy].AddLine(x0,y0,x1,y1);
}

void PaintCmds::AddPoint(qreal& x0,qreal& y0)
{
        Cmds[iStrategy].AddPoint(x0,y0);
}

void PaintCmds::AddText(qreal& x0,qreal& y0,QString s)
{
        Cmds[iStrategy].AddText(x0,y0,s);
}

void PaintCmds::AddVelo(qreal& x0,qreal& y0,qreal& x1,qreal& y1)
{
     Cmds[iStrategy].AddVelo(x0,y0,x1,y1);;
}

void PaintCmds::AddRobot(qreal &x0, qreal &y0, qreal &z0)
{
    Cmds[iStrategy].AddRobot(x0,y0,z0);
}

void PaintCmds::StrategySwitchCmds()
{
        int itemp;
        //策略切换 between iIdle and iStrategy
        IdleMutex.lock();
//        itemp=iIdle;
//        iIdle=iStrategy;
//        iStrategy=itemp;
        Cmds[iStrategy].Clear();
        IdleMutex.unlock();
}

void PaintCmds::GuiSwitchCmds()
{
        qDebug()<<"GuiSwitch";
        int itemp;
        //GUI切换
        IdleMutex.lock();
        itemp=iIdle;
        iIdle=iGui;
        iGui=itemp;
        Cmds[iIdle].Clear();
        IdleMutex.unlock();
}

void PaintCmds::ExecCmds(QPainter * painter)
{
        Cmds[iStrategy].ExecCmds(painter);
}
