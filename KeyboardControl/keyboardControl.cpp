#include "keyboardControl.h"
#include "../RouteRegistration/routeRegistrationUtils.h"
#include "../RouteRecordPlayer/routeRecordPlayUtils.h"
#include <sys/select.h> 
#include <termios.h> 

using namespace std;

VisionVoyager* KeyboardControl::robotVisionVoyager = nullptr;
bool KeyboardControl::F11_pressed = false;
bool KeyboardControl::keyboard_control_active = false;

void KeyboardControl::set_robot(VisionVoyager* robot)
{
    KeyboardControl::robotVisionVoyager = robot;
}

void KeyboardControl::initialize_keyboard_control()
{
    /* curses.h specific */
    /* Initializes the curses library and sets up the data structures */
    initscr();
    /* Disables line buffering and processes input character by character */
    raw(); 
    /* Enables keypad input for the standard screen */
    keypad(stdscr, TRUE); 
}

 void KeyboardControl::shutdown_keyboard_control()
 {
    endwin();
 }

 void KeyboardControl::F11_listening_loop()
 {
    chrono::seconds interval(5);
    auto start_time = chrono::steady_clock::now();

    struct termios initial_settings, new_settings;
    tcgetattr(0, &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_cc[VMIN] = 0; 
    new_settings.c_cc[VTIME] = 0; 
    tcsetattr(0, TCSANOW, &new_settings);

    while((!F11_pressed) && (interval > chrono::steady_clock::now() - start_time))
    {   
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(0, &fds);
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        int ready = select(1, &fds, NULL, NULL, &timeout); 
        if (ready > 0) 
        {
            int ch = getch(); 
            if (ch != ERR) 
            { 
                if (ch == KEY_F(11)) 
                {
                    F11_pressed = true;
                    logFile << log_time() << "F11 Pressed! " << endl;
                }
            }
            else
            {
                logFile << log_time() << "Error: Problem with listening for F11 input" << endl;
            }
        }
    }
    logFile << log_time() << "F11 Not Pressed!" << endl;
 }

 bool KeyboardControl::get_F11_pressed()
 {
    return F11_pressed;
 }

 const std::unordered_map<std::string, int> key_map 
 {
    {"Left", KEY_LEFT},
    {"Up", KEY_UP},
    {"Right", KEY_RIGHT},
    {"Down", KEY_DOWN},
    {"1", '1'},
    {"2", '2'},
    {"3", '3'},
    {"4", '4'},
    {"5", '5'},
    {"6", '6'},
    {"P", 'p'},
    {"O", 'o'},
    {"W", 'w'},
    {"A", 'a'},
    {"S", 's'},
    {"D", 'd'},
    {"F", 'f'},
    {"R", 'r'},
    {"E", 'e'},
    {"X", 'x'}
};

int KeyboardControl::map_key(const string& key)
{
    auto it = key_map.find(key);

    if (it != key_map.end()) 
    {
        return it->second;
    } 
    else 
    {
        return -1;
    }
}

void KeyboardControl::keyboard_listening_loop(string key_s)
{
    int key = 0;

    if(true == keyboard_control_active)
    {   
        key = map_key(key_s);
        cout << key_s << "   " << key << endl;

        switch (key) 
        {
            case 'w':
                DEBUG_MSG("Ai apasat tasta 'w'");
                robotVisionVoyager->move_forward();
                RouteRegistration::register_current_move("forward", robotVisionVoyager->get_speed());
                RouteRegistration::set_moving_state(MovingState::MOVING_FORWARD);
                break;
            case 's':
                DEBUG_MSG("Ai apasat tasta 's'");
                robotVisionVoyager->move_backward();
                RouteRegistration::register_current_move("backward", robotVisionVoyager->get_speed());
                RouteRegistration::set_moving_state(MovingState::REVERSING);
                break;
            case 'a':
                DEBUG_MSG("Ai apasat tasta 'a'");
                robotVisionVoyager->turn_left_one_degree();
                RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
                break;
            case 'd':
                DEBUG_MSG("Ai apasat tasta 'd'");
                robotVisionVoyager->turn_right_one_degree();
                RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
                break;
            case 'f':
                DEBUG_MSG("Ai apasat tasta 'f'");
                robotVisionVoyager->stop();
                RouteRegistration::register_current_move("stop");
                RouteRegistration::set_moving_state(MovingState::STATIONARY);
                break;
            case 'x': 
                DEBUG_MSG("Ai apasat tasta 'x'");
                robotVisionVoyager->set_dir_angle(0);
                RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
                break;
            case KEY_UP:
                DEBUG_MSG("Ai apasat tasta '↑'");
                robotVisionVoyager->camera_tilt_up_one_degree();
                RouteRegistration::register_current_move("set_camera_tilt_angle", robotVisionVoyager->get_camera_tilt_angle());
                break;
            case KEY_DOWN:
                DEBUG_MSG("Ai apasat tasta '↓'");
                robotVisionVoyager->camera_tilt_down_one_degree();
                RouteRegistration::register_current_move("set_camera_tilt_angle", robotVisionVoyager->get_camera_tilt_angle());
                break;
            case KEY_LEFT:
                DEBUG_MSG("Ai apasat tasta '←'");
                robotVisionVoyager->camera_pan_left_one_degree();
                RouteRegistration::register_current_move("set_camera_pan_angle", robotVisionVoyager->get_camera_pan_angle());
                break;
            case KEY_RIGHT:
                DEBUG_MSG("Ai apasat tasta '→'");
                robotVisionVoyager->camera_pan_right_one_degree();
                RouteRegistration::register_current_move("set_camera_pan_angle", robotVisionVoyager->get_camera_pan_angle());
                break;
            case '1':
                DEBUG_MSG("Ai apasat tasta '1'");
                robotVisionVoyager->turn_left_max();
                RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
                break;
            case '2':
                DEBUG_MSG("Ai apasat tasta '2'.\n");
                robotVisionVoyager->turn_right_max();
                RouteRegistration::register_current_move("set_dir_servo_angle", robotVisionVoyager->get_dir_angle());
                break;
            case '3':
                DEBUG_MSG("Ai apasat tasta '3'.\n");
                robotVisionVoyager->camera_pan_left_max();
                RouteRegistration::register_current_move("set_camera_pan_angle", robotVisionVoyager->get_camera_pan_angle());
                break;
            case '4':
                DEBUG_MSG("Ai apasat tasta '4'.\n");
                robotVisionVoyager->camera_pan_right_max();
                RouteRegistration::register_current_move("set_camera_pan_angle", robotVisionVoyager->get_camera_pan_angle());
                break;
            case '5':
                DEBUG_MSG("Ai apasat tasta '5'.\n");
                robotVisionVoyager->camera_tilt_up_max();
                RouteRegistration::register_current_move("set_camera_tilt_angle", robotVisionVoyager->get_camera_tilt_angle());
                break;
            case '6':
                DEBUG_MSG("Ai apasat tasta '6'.\n");
                robotVisionVoyager->camera_tilt_down_max();
                RouteRegistration::register_current_move("set_camera_tilt_angle", robotVisionVoyager->get_camera_tilt_angle());
                break;
            case 'o':
                DEBUG_MSG("Ai apasat tasta 'o'.\n");
                robotVisionVoyager->speed_up();
                RouteRegistration::register_speed_change(robotVisionVoyager->get_speed());
                break;
            case 'p':
                DEBUG_MSG("Ai apasat tasta 'p'.\n");
                robotVisionVoyager->speed_down();
                RouteRegistration::register_speed_change(robotVisionVoyager->get_speed());
                break;
            case 't':
                DEBUG_MSG("Ai apasat tasta 't'.\n");
                RouteRecordPlayer::play_route("record_Route_No_1");
                /* @ToDo */
                break;
            case 'r':
                DEBUG_MSG("Ai apasat tasta 'r'.\n");
                RouteRegistration::set_register_enabled_true();
                break;
            case 'e':
                DEBUG_MSG("Ai apasat tasta 'e'.\n");
                RouteRegistration::end_registration();
                break;
            case 'q':
                DEBUG_MSG("Programul se inchide.\n");
                robotVisionVoyager->stop();
                keyboard_control_active = false;
                break;
            default:
                /* Do nothing */
                break;
        }
    }
}

void KeyboardControl::set_keyboard_control_active(bool state)
{
    keyboard_control_active = state;
}

bool KeyboardControl::get_keyboard_control_active()
{
    return keyboard_control_active;
}