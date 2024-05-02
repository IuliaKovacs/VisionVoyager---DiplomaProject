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
    static const unordered_map<string, int> keyMap;
    static int map_key(const string& key);
    static bool keyboard_control_active;

public:
    static void set_robot(VisionVoyager* robot);
    static void initialize_keyboard_control();
    static void shutdown_keyboard_control();
    static void F11_listening_loop();
    static bool get_F11_pressed();
    static void keyboard_listening_loop(string key_s);
    static void set_keyboard_control_active(bool state);
    static bool get_keyboard_control_active();
};

#endif //KEYBOARD_CONTROL