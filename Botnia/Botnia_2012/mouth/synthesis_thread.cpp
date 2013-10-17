// ************************************************************************************************
//     Copyright 2012 Gao Yuan (robotics-computing.tk)
//     
//     This software is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
//     Additional license information:
// 
//  **********************************************************************************************/

/**    @file    - synthesis_thread.cpp
  *
  *    @class   - SynthesisThread
  *
  *    @brief   -
  *
  *    @author  -
  *
  *    @date    - 12/27/2012
  *
  *    @details -
  *
  *    Project  - %PROJECT%
  */


#include "synthesis_thread.h"
#include "ear/voce.h"

using namespace Mouth;


SynthesisThread::SynthesisThread(QObject *parent) :
    QThread(parent)
{
}
SynthesisThread::~SynthesisThread()
{
}
void SynthesisThread::InitializeServer()
{
    voce::init("../../../lib", true, false, "", "");
}
void SynthesisThread::Say()
{
    std::string s = to_say_.toStdString();
    voce::synthesize(s);
}
void SynthesisThread::Say(QString to_say)
{
    std::string s = to_say.toStdString();
    voce::synthesize(s);
}
