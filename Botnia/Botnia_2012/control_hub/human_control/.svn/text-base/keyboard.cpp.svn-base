#include "keyboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

Keyboard::Keyboard()
{
}
Keyboard::~Keyboard()
{
}
void Keyboard::PortInitialization(void)
{
    tcgetattr( fileno( stdin ), &oldSettings );
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr( fileno( stdin ), TCSANOW, &newSettings );
}
void Keyboard::Finalize(void)
{
    tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
}
void Keyboard::KeyboardControl(void)
{
    int waiting_for_sequence = 0;
    char c;
    bool done = false;
    while ( !done )
    {
        fd_set set;
        struct timeval tv;

        tv.tv_sec = 0;
        tv.tv_usec = 50000;

     //   FD_ZERO( &set );
        FD_SET( fileno( stdin ), &set );

        int res = select( fileno( stdin )+1, &set, NULL, NULL, &tv );

        if( res > 0 )
        {

            read( fileno( stdin ), &c, 1 );
            printf("%d\n",c);
            switch(c)
            {
            case 27:
                waiting_for_sequence++;
                break;
            case 65:
                if(waiting_for_sequence == 2)
                {
                    waiting_for_sequence = 0 ;
                    printf("Key up");
                }
                break;
            case 66:
                if(waiting_for_sequence == 2)
                {
                    waiting_for_sequence = 0 ;
                    printf("Key down");
                }
                break;
            case 68:
                if(waiting_for_sequence == 2)
                {
                    waiting_for_sequence = 0 ;
                    printf("Key left");
                }
                break;
            case 67:
                if(waiting_for_sequence == 2)
                {
                    waiting_for_sequence = 0 ;
                    printf("Key right");
                }
                break;
            case 91:
                if(waiting_for_sequence == 1)
                    waiting_for_sequence ++;
                break;
            case 97:
                printf("Key left");
                break;
            case 100:
                printf("Key right");
                break;
            case 119:
                printf("Key up");
                break;
            case 115:
                printf("Key down");
                break;
            case 120:
                done = true;
                break;
            default:
                break;
            }
        }
        else if( res < 0 )
        {
            perror( "select error" );
            break;
        }
        else
        {
            printf( "wait\n" );
        }
    }
}


