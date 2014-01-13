#define DECT false

#define MAIN_PROGRAM
//#define UNIT_TEST_SERIAL
//#define UNIT_TEST_SLIP_PACKAGE

//#define UNIT_TEST_KINECT
//#define UNIT_TEST_JOYSTICK

#define LU_TEST


#ifdef UNIT_TEST_KINECT

#include "control_hub/human_control/kinect.h"
int main(void)
{
    Kinect_Name_Space::Kinect kinect = Kinect_Name_Space::Kinect();
    kinect.Run();
}
#endif

#ifdef UNIT_TEST_SLIP_PACKAGE
#include "wireless/serial_server.h"
#include <QtDebug>
#include "net/web_data_model/webdatamodel.h"
#include "net/net_webserver_send_thread.h"

int main(void)
{

    // start broadcasting packets to web server..
    NetWebserverSendThread* net_webserver_send_thread = new NetWebserverSendThread();
    net_webserver_send_thread->start(QThread::TimeCriticalPriority);


//    qDebug() << "here...";
//    WebDataModel* dataModel;
//    QUdpSocket *udpSocket = new QUdpSocket();;
//    QString msg = dataModel->createWebData();
//    int port = 5555;
//
//    int length = 0;
//    if(msg == "")
//    {
////        return;
//    }
//
//
//    if((length=udpSocket->writeDatagram(msg.toLatin1(),msg.length(), QHostAddress::Broadcast, port))!=msg.length())
//    {
//        qDebug() << "Failed!";
//    }
//    else{
//        qDebug() << "Success";
//    }




    SerialServer serial_server_;
    serial_server_.RecievePackage();



}




#endif




#ifdef UNIT_TEST_SERIAL


#include "wireless/serial_operations.h"
int main(void)
{
    RobotParamters robot_paramters;
    robot_paramters = ClearRobotParameters(robot_paramters);
    robot_paramters.x_velocity = 10;
    SerialOperations serial_operation = SerialOperations(robot_paramters);
    serial_operation.SendPackage();
}

#endif


#if 0



// DECT TESTING MAIN FILE

#include <stdio.h>
#include "wireless/DECT_operation.h"
#include "wireless/transparent_operation.h"
#include <sys/wait.h>
int main(void)
{
    RobotParamters robot_parametres;
    ClearRobotParameters(robot_parametres);
    robot_parametres.index = 1;
    WirelessRobot robot = WirelessRobot(robot_parametres);
    SendDECTPackage package;
    QByteArray temp_array;
    unsigned char mac_address[6] = {0xff, 0xff,0xff,0xff, 0xff,0xff};
    char * input_string = (char*)"";
    char * finalhash = new char[strlen(input_string)/2+1];
    finalhash[strlen(input_string)/2] = '\0';
   // char * str = string_to_hex(input_string,finalhash);
    printf("%d,%x\n",strlen(input_string)/2,finalhash[1]);
    temp_array = package.FormByteCommand(temp_array,&robot);
    package.PortInitialization();
    int i = 0;
    int temp_size = temp_array.size();
    unsigned char *temp_pointer = new unsigned char(temp_size);
    memcpy(temp_pointer, temp_array.data(), temp_size);

    while(1)
    {
    package.SendPackage(mac_address, temp_pointer,temp_size);
  //  printf("string length: %d\n",sizeof(str));
    sleep(1);
    i++;
    if(i==2) break;
    }
    return 0;

}
#endif
// TRANSPARENT MAIN FILE
#if 0
int main(void)
{
    RobotParamters robot_parameters;
    robot_parameters = ClearRobotParameters(robot_parameters);
    robot_parameters.index = 0;
    robot_parameters.x_velocity = 0;
    robot_parameters.y_velocity = 0;
    WirelessRobot robot = WirelessRobot(robot_parameters);
    TransparentPackage package;
    QByteArray temp_array;
    temp_array = package.FormByteCommand(temp_array,&robot);

    int fd=package.PortInitialization();
    int temp_size = temp_array.size();
    unsigned char *temp_pointer = new unsigned char(temp_size);
    memcpy(temp_pointer, temp_array.data(), temp_size);
    while(1)
    {
        package.SendPackage(temp_pointer,temp_size,fd);
        break;
    }
    return 0;

}
#endif


#if 0
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
#endif


// TRANSPARENT WITH KEYBOARD
#if 0
#include "control_hub/human_control/keyboard.h"
int main()
{
    Keyboard keyboard = Keyboard();
    keyboard.PortInitialization();
    keyboard.KeyboardControl();
    keyboard.Finalize();
    return 0;
}

#endif


#ifdef UNIT_TEST_JOYSTICK

#include "control_hub/human_control/joystick_gao.h"

#include "wireless/serial_server.h"

int main()
{
    SerialServer serial_server = SerialServer();

    Joystick joystick = Joystick();
    joystick.open_joystick();
    RobotParamters robot_parameters;
    robot_parameters = ClearRobotParameters(robot_parameters);
    joystick.JoystickControl(&serial_server);

/*
    robot_parameters.x_velocity = joystick.set_joystick_x_axis();
    robot_parameters.y_velocity = joystick.set_joystick_y_axis();

    WirelessRobot robot = WirelessRobot(robot_parameters);
    robot.set_x_velocity(robot_parameters.x_velocity);
    robot.set_y_velocity(robot_parameters.y_velocity);

    TransparentPackage package;
    QByteArray temp_array;
    temp_array = package.FormByteCommand(temp_array,&robot);

    int fd=package.PortInitialization();
    int temp_size = temp_array.size();
    unsigned char *temp_pointer = new unsigned char(temp_size);
    memcpy(temp_pointer, temp_array.data(), temp_size);
    while(1)
    {
    package.SendPackage(temp_pointer,temp_size,fd);
    break;
    }
*/

    robot_parameters.x_velocity = joystick.joystick_x_axis();
    robot_parameters.y_velocity = joystick.joystick_y_axis();
    robot_parameters.index = 0;

    WirelessRobot robot = WirelessRobot(robot_parameters);
    robot.set_x_velocity(robot_parameters.x_velocity);
    robot.set_y_velocity(robot_parameters.y_velocity);

    TransparentOperation package;
    QByteArray temp_array;
    temp_array = package.FormByteCommand(temp_array,&robot);

    int fd=package.PortInitialization();
    int temp_size = temp_array.size();
    unsigned char *temp_pointer = new unsigned char(temp_size);
    memcpy(temp_pointer, temp_array.data(), temp_size);
    while(1)
    {
    package.SendPackage(temp_pointer,temp_size,fd);
    break;
    }


    return 0;
}
#endif


#if 0
// The Straight Path Testing
#include "control_hub/test_control/path_line.h"
#include "wireless/transparent_operation.h"
#include "wireless/wireless_robot.h"
int main ()
{
    RobotParamters robot_parameters;
    robot_parameters = ClearRobotParameters(robot_parameters);
    WirelessRobot robot = WirelessRobot(robot_parameters);
    TransparentPackage packege;
    PathLine pathline = PathLine();
    pathline.run_on_path(robot,packege);
}

#endif


//#ifdef MAIN_PROGRAM
#if 0

#include <QApplication>
#include <QSplashScreen>
// things that were oringinal before the main function
#include "user_interface/field_related/field_global_function.h"
#include "user_interface/strategy_control_window.h"

#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/anticipation_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/special_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/defense_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.h"




///////////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QPixmap pixmap("user_interface/robocup_logo.png");
    QSplashScreen *splash = new QSplashScreen(pixmap,Qt::WindowStaysOnTopHint);
    splash->show();

    Qt::Alignment bottom_right = Qt::AlignBottom | Qt::AlignRight;
    splash->showMessage(QObject::tr("Registering tactics..."),bottom_right,Qt::black);
    // register the tactics
    RegisterTactics();
    splash->showMessage(QObject::tr("Loading configerations..."),bottom_right,Qt::black);
    application_path=application.applicationDirPath();
    MainWindow strategy_window;
    strategy_window.show();
    sleep(2);
    splash->close();
    delete splash;
    splash = NULL;
    return application.exec();
}
#endif

#ifdef LU_TEST

#include <QApplication>
#include <QSplashScreen>
// things that were oringinal before the main function
#include "user_interface/field_related/field_global_function.h"
#include "user_interface/strategy_control_window.h"

#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/anticipation_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/special_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/defense_tactics.h"
#include "control_hub/computer_control/intelligence/strategy_extutor/tactic/ball_tactics.h"




///////////////////////////////////////////////////////////////////////////////////


int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QPixmap pixmap("user_interface/robocup_logo.png");
    QSplashScreen *splash = new QSplashScreen(pixmap,Qt::WindowStaysOnTopHint);
    splash->show();

    Qt::Alignment bottom_right = Qt::AlignBottom | Qt::AlignRight;
    splash->showMessage(QObject::tr("Registering tactics..."),bottom_right,Qt::black);
    // register the tactics
    RegisterTactics();
    splash->showMessage(QObject::tr("Loading configerations..."),bottom_right,Qt::black);
    application_path=application.applicationDirPath();
    MainWindow strategy_window;
    strategy_window.show();
    sleep(2);
    splash->close();
    delete splash;
    splash = NULL;



    return application.exec();
}

#endif
