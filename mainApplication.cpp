#include "VisionVoyagerMoves/visionVoyager.h"
#include "RouteRegistration/routeRegistrationUtils.h"
#include "RouteRecordPlayer/routeRecordPlayUtils.h"
#include "KeyboardControl/keyboardControl.h"
#include "LineFollower/lineFollower.h"
#include "ObstacleAvoidance/obstacleAvoidanceUtils.h"
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>


using namespace std;


void initilize_main_app()
{   
    KeyboardControl::initialize_keyboard_control();
    RouteRegistration::initialize_route_registration();
}

void terminate_main_app()
{
    KeyboardControl::shutdown_keyboard_control();
}

int main() 
{    
    initilize_main_app();

    /* pybind11 specific - starts the interpreter and maintains it alive */
    py::scoped_interpreter guard{}; 

    VisionVoyager robot = VisionVoyager();
    RouteRecordPlayer::setRobot(&robot);
    KeyboardControl::setRobot(&robot);
    LineFollower::setRobot(&robot);
    ObstacleAvoidance::setRobot(&robot);
    

    // RouteRegistration::set_register_enabled_true();

    // LineFollower::follow_line();

    // robot.stop();

    // KeyboardControl::keyboard_listening_loop();
    
    ObstacleAvoidance::avoid_simple_obstacle();

    terminate_main_app();

    return 0;
}
