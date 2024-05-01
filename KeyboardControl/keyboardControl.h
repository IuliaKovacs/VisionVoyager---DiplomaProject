#ifndef KEYBOARD_CONTROL
#define KEYBOARD_CONTROL

#include "../VisionVoyagerMoves/visionVoyager.h"
#include "../ApplicationModule/application_utils.h"
#include <curses.h>
#include <iostream>
#include <chrono>

class KeyboardControl
{   
    static VisionVoyager* robotVisionVoyager;
    static bool F11_pressed;

public:
    static void set_robot(VisionVoyager* robot);
    static void initialize_keyboard_control();
    static void shutdown_keyboard_control();
    static void keyboard_listening_loop();
    static void F11_listening_loop();
    static bool get_F11_pressed();
};

#endif //KEYBOARD_CONTROL