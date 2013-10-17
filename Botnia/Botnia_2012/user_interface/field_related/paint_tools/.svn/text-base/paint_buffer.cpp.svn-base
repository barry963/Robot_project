#include "paint_buffer.h"

#include "paint_buffer.h"

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

void PaintCmd::ExecCmds(QPainter * painter)
{
        if (iLineCount)
        {
                painter->drawLines(lines,iLineCount);
        }
        if (iPointCount)
        {
                painter->drawPoints(points,iPointCount);
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

void PaintCmds::StrategySwitchCmds()
{
        int itemp;
        //策略切换
        IdleMutex.lock();
        itemp=iIdle;
        iIdle=iStrategy;
        iStrategy=itemp;
        Cmds[iStrategy].Clear();
        IdleMutex.unlock();
}

void PaintCmds::GuiSwitchCmds()
{
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
        Cmds[iGui].ExecCmds(painter);
}
