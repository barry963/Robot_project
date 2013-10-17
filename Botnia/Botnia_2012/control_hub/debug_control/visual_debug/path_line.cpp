//#include "path_line.h"



//PathLine::PathLine()
//{

//}

//PathLine::~PathLine()
//{
//}


//void PathLine::run_on_path(WirelessRobot robot, TransparentPackage package){
//    int done = 0;
//    QByteArray temp_array;
//    int fd_wireless=package.PortInitialization();
//    while (!done) {
//        //Change the default speed to be predefined speed.
//        robot.set_x_velocity(PATH_TEST_SPEED);
//        //Form the command
//        temp_array = package.FormByteCommand(temp_array,&robot);
//        int temp_size = temp_array.size();
//        unsigned char *temp_pointer = new unsigned char(temp_size);
//        memcpy(temp_pointer, temp_array.data(), temp_size);
//        // Send the command
//        while(1)
//        {
//        package.SendPackage(temp_pointer,temp_size,fd_wireless);
//        break;
//        }
//    }//end of while
//}
