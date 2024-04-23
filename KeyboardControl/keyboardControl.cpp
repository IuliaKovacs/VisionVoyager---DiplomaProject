#include "keyboardControl.h"
#include "../RouteRegistration/routeRegistrationUtils.h"
#include "../RouteRecordPlayer/routeRecordPlayUtils.h"


using namespace std;

VisionVoyager* KeyboardControl::robotVisionVoyager = nullptr;

void KeyboardControl::set_robot(VisionVoyager* robot)
{
    KeyboardControl::robotVisionVoyager = robot;
}

void KeyboardControl::keyboard_listening_loop()
{
    int key = 0;
    int loop_on = 1;    

    while (loop_on == 1) {
        scanf(" %d", &key);

        switch ((key = getch())) {
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
                loop_on = 0;
                break;
            default:
                /* Do nothing */
                break;
        }
    }
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