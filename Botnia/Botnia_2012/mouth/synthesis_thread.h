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

/**    @file    - synthesis_thread.h
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


#ifndef SYNTHESIS_THREAD_H
#define SYNTHESIS_THREAD_H
#include <QThread>
namespace Mouth
{
    class SynthesisThread : public QThread
    {
        Q_OBJECT
    private:
        // initialize the server
        void InitializeServer();
        // string about to say
        QString to_say_;
        // thread status
        bool stopped_;
        // string that was just saided
        QString remembered_;



    public:
        // constructer
        explicit SynthesisThread(QObject *parent = 0);
        ~SynthesisThread();
        // say the previously stored sring
        void Say();
        // say the new string
        void Say(QString to_say);
        // setter of to_say
        void set_to_say(QString to_say){to_say_ = to_say;}
        // getter of to_say
        QString to_say(){return to_say_;}
        // setter of stopped
        void set_stopped(bool stopped){ stopped_ = stopped;}
        // getter of stopped
        bool stopped(){return stopped_;}


    };
}


#endif // SYNTHESIS_THREAD_H
