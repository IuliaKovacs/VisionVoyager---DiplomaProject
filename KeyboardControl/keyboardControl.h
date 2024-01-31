#ifndef KEYBOARD_CONTROL
#define KEYBOARD_CONTROL

#include "../VisionVoyagerMoves/visionVoyager.h"
#include <curses.h>
#include <iostream>

class KeyboardControl
{   
    static VisionVoyager* robotVisionVoyager;

public:
    static void setRobot(VisionVoyager* robot);
    static void initialize_keyboard_control();
    static void shutdown_keyboard_control();
    static void display_moves_dictionary();
    static void keyboard_listening_loop();
};

#endif //KEYBOARD_CONTROL