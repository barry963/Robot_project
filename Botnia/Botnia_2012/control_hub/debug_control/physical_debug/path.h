#ifndef PATH_H
#define PATH_H
#define PATH_TEST_SPEED 15
//#include "../robot.h"
#include "../../wireless/transparent_package.h"

class Path
{
private:
    char* path_name_;
public:
    Path();
    virtual ~Path();
    void set_path_name(char *path_name){path_name_ = path_name;}
    char* path_name(void){return path_name_;}

};
#endif // PATH_H
