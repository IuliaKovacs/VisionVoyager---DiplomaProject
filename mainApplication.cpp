#include "VisionVoyagerMoves/visionVoyager.h"
#include "RouteRegistration/routeRegistrationUtils.h"
#include "RouteRecordPlayer/routeRecordPlayUtils.h"
#include "KeyboardControl/keyboardControl.h"
#include "LineFollower/lineFollower.h"
#include "ObstacleAvoidance/obstacleAvoidanceUtils.h"
#include "CameraModule/cameraModule.h"
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>



using namespace std;


void initilize_main_app()
{   
    KeyboardControl::initialize_keyboard_control();
    RouteRegistration::initialize_route_registration();
    CameraModule::initialize_camera_module();
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
    
    
    // ObstacleAvoidance::simulate_real_case();
    // ObstacleAvoidance::choose_avoiding_side();
    // ObstacleAvoidance::avoid_simple_obstacle_right_side();

    // CameraModule::display_camera_capture();

    cout << endl << endl << endl;

    // CameraModule::create_csv_database_file();

    // int result = CameraModule::recognize_face("../CameraModule/mgk.jpg");
    // cout << result << endl; 

    // int result1 = CameraModule::recognize_face("../CameraModule/taylor2.jpg");
    // cout << result1 << endl; 

    // int result2 = CameraModule::recognize_face("../CameraModule/chair.jpg");
    // cout << result2 << endl;  

    // int result3 = CameraModule::recognize_face("../CameraModule/taylor.jpg");
    // cout << result3 << endl;  

    // int result4 = CameraModule::recognize_face("../CameraModule/mgk2.jpg");
    // cout << result4 << endl; 

    
//     if (CameraModule::detect_lowerbody("../CameraModule/input_image.jpg")) 
//     {
//          cout << "present" <<  endl;
//     } 
//     else 
//     {
//          cout << "not present" <<  endl;
//     }

//     if (CameraModule::detect_lowerbody("../CameraModule/mgk.jpg")) 
//     {
//          cout << "present" <<  endl;
//     } 
//     else 
//     {
//          cout << "not present" <<  endl;
//     }

//     if (CameraModule::detect_lowerbody("../CameraModule/lower.png")) 
//     {
//          cout << "present" <<  endl;
//     } 
//     else 
//     {
//          cout << "not present" <<  endl;
//     }

     int photo_no = 0;

     while(true)
     {
          CameraModule::capture_image("../CameraModule/CapturedImages");
          photo_no++;
          this_thread::sleep_for(std::chrono::milliseconds(1000));

          if(photo_no == 20)
          {
               break;
          }
     }

    // CameraModule::add_new_recognized_subject();

    terminate_main_app();

    return 0;
}
