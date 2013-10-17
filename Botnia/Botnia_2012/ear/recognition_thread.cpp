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

/**    @file    - recognition_thread.cpp
  *
  *    @class   - RecognitionThread
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


#include "recognition_thread.h"
#include "ear/voce.h";

using namespace Ear;

RecognitionThread::RecognitionThread(QObject *parent) :
    QThread(parent)
{
    stoped_ = false;
    InitializeServer();
}
RecognitionThread::~RecognitionThread()
{}

void RecognitionThread::InitializeServer()
{
    voce::init("../../../lib", false, true, "./grammar", "digits");
}
QString RecognitionThread::Hear()
{
    std::string s = "";
    if(!stoped_)
    {
        usleep(200);
        while (voce::getRecognizerQueueSize() > 0)
        {
                s = voce::popRecognizedString();
                if (std::string::npos != s.rfind("quit"))
                {
                        stoped_ = true;
                }
                std::cout << "You said: " << s << std::endl;
        }

    }
    return QString::fromStdString(s);
}

