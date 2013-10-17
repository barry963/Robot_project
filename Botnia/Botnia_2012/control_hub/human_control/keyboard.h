#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

class Keyboard
{
    struct termios oldSettings, newSettings;
public:
    Keyboard();
    ~Keyboard();
    void PortInitialization(void);
    int ReturnPressedKey(void);
    void KeyboardControl(void);
    void Finalize(void);

};

#endif // KEYBOARD_H
