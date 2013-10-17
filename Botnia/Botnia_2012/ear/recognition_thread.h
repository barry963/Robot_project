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

/**    @file    - recognition_thread.h
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


#ifndef RECOGNITION_THREAD_H
#define RECOGNITION_THREAD_H

#include <QThread>



namespace Ear
{
    class RecognitionThread : public QThread
    {
        Q_OBJECT
    private:
        QString remembered_;
        bool stoped_;
        void InitializeServer();
        QString Hear();

    public:
        explicit RecognitionThread(QObject *parent = 0);
        ~RecognitionThread();
        void set_remembered();
        QString remembered();


        //void Hear();

    };
}

#endif // RECOGNITION_THREAD_H
