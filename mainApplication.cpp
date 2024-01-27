#include "VisionVoyagerMoves/visionVoyager.h"
#include "RouteRegistration/routeRegistrationUtils.h"
#include <curses.h>
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>

using namespace std;

void initilize_main_app()
{   
    /* curses.h specific */
    /* Initializes the curses library and sets up the data structures */
    initscr();
    /* Disables line buffering and processes input character by character */
    raw(); 
    /* Enables keypad input for the standard screen */
    keypad(stdscr, TRUE); 

}


/*Moves Dictionary:
    O: speed up
    P: speed down
    W: forward
    S: backward
    A: turn left one
    D：turn right
    F: stop
    T: take photo
    ↑: Camera tilt up (Up arrow)
    ↓: Camera tilt down (Down arrow)
    →: Camera pan right (Right arrow)
    ←: Camera pan left (Left arrow)
    1: turn left max degree
    2: turn right max degree
    3: Camera pan left max
    4: Camera pan right max
    5: Camera tilt up max
    6: Camera tilt down max
    Q: quit  */


int main() 
{    
    initilize_main_app();

    /* pybind11 specific - starts the interpreter and maintains it alive */
    py::scoped_interpreter guard{}; 

    VisionVoyager robot = VisionVoyager();
    RouteRegistration routeRegistration = RouteRegistration();

    int key;
    int loop_on = 1;

    

    while (loop_on == 1) {
        scanf(" %d", &key);

        switch ((key = getch())) {
            case 'w':
                DEBUG_MSG("Ai apasat tasta 'w'");
                robot.move_forward();
                routeRegistration.register_current_move("forward", robot.get_speed());
                routeRegistration.set_moving_state(MovingState::MOVING_FORWARD);
                break;
            case 's':
                DEBUG_MSG("Ai apasat tasta 's'");
                robot.move_backward();
                routeRegistration.register_current_move("backward", robot.get_speed());
                routeRegistration.set_moving_state(MovingState::REVERSING);
                break;
            case 'a':
                DEBUG_MSG("Ai apasat tasta 'a'");
                robot.turn_left_one_degree();
                routeRegistration.register_current_move("set_dir_servo_angle", robot.get_dir_angle());
                break;
            case 'd':
                DEBUG_MSG("Ai apasat tasta 'd'");
                robot.turn_right_one_degree();
                routeRegistration.register_current_move("set_dir_servo_angle", robot.get_dir_angle());
                break;
            case 'f':
                DEBUG_MSG("Ai apasat tasta 'f'");
                robot.stop();
                routeRegistration.register_current_move("stop");
                routeRegistration.set_moving_state(MovingState::STATIONARY);
                break;
            case KEY_UP:
                DEBUG_MSG("Ai apasat tasta '↑'");
                robot.camera_tilt_up_one_degree();
                routeRegistration.register_current_move("set_camera_tilt_angle", robot.get_camera_tilt_angle());
                break;
            case KEY_DOWN:
                DEBUG_MSG("Ai apasat tasta '↓'");
                robot.camera_tilt_down_one_degree();
                routeRegistration.register_current_move("set_camera_tilt_angle", robot.get_camera_tilt_angle());
                break;
            case KEY_LEFT:
                DEBUG_MSG("Ai apasat tasta '←'");
                robot.camera_pan_left_one_degree();
                routeRegistration.register_current_move("set_camera_pan_angle", robot.get_camera_pan_angle());
                break;
            case KEY_RIGHT:
                DEBUG_MSG("Ai apasat tasta '→'");
                robot.camera_pan_right_one_degree();
                routeRegistration.register_current_move("set_camera_pan_angle", robot.get_camera_pan_angle());
                break;
            case '1':
                DEBUG_MSG("Ai apasat tasta '1'");
                robot.turn_left_max();
                routeRegistration.register_current_move("set_dir_servo_angle", robot.get_dir_angle());
                break;
            case '2':
                DEBUG_MSG("Ai apasat tasta '2'.\n");
                robot.turn_right_max();
                routeRegistration.register_current_move("set_dir_servo_angle", robot.get_dir_angle());
                break;
            case '3':
                DEBUG_MSG("Ai apasat tasta '3'.\n");
                robot.camera_pan_left_max();
                routeRegistration.register_current_move("set_camera_pan_angle", robot.get_camera_pan_angle());
                break;
            case '4':
                DEBUG_MSG("Ai apasat tasta '4'.\n");
                robot.camera_pan_right_max();
                routeRegistration.register_current_move("set_camera_pan_angle", robot.get_camera_pan_angle());
                break;
            case '5':
                DEBUG_MSG("Ai apasat tasta '5'.\n");
                robot.camera_tilt_up_max();
                routeRegistration.register_current_move("set_camera_tilt_angle", robot.get_camera_tilt_angle());
                break;
            case '6':
                DEBUG_MSG("Ai apasat tasta '6'.\n");
                robot.camera_tilt_down_max();
                routeRegistration.register_current_move("set_camera_tilt_angle", robot.get_camera_tilt_angle());
                break;
            case 'o':
                DEBUG_MSG("Ai apasat tasta 'o'.\n");
                robot.speed_up();
                routeRegistration.register_speed_change(robot.get_speed());
                break;
            case 'p':
                DEBUG_MSG("Ai apasat tasta 'p'.\n");
                robot.speed_down();
                routeRegistration.register_speed_change(robot.get_speed());
                break;
            case 't':
                DEBUG_MSG("Ai apasat tasta 't'.\n");
                /* @ToDo */
                break;
            case 'r':
                DEBUG_MSG("Ai apasat tasta 'r'.\n");
                routeRegistration.set_register_enabled_true();
                break;
            case 'e':
                DEBUG_MSG("Ai apasat tasta 'e'.\n");
                routeRegistration.end_registration();
                break;
            case 'q':
                DEBUG_MSG("Programul se inchide.\n");
                robot.stop();
                loop_on = 0;
                break;
            default:
                /* Do nothing */
                break;
        }
    }

    endwin();
    
    return 0;
}
