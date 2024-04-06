#include "ApplicationModule/application.h"


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

    /* @ToDo make initialization look clean */
    VisionVoyager robot = VisionVoyager();
    RouteRecordPlayer::setRobot(&robot);
    KeyboardControl::setRobot(&robot);
    LineFollower::setRobot(&robot);
    ObstacleAvoidance::setRobot(&robot);



    /* StandBy/Sleep state until "start" or "hello" is recognized */
    // VoiceRecognition::loop_recognition();

    set_language_RO();

    display_hello_message();

    bool option_flag = false;
    string option;

    while(option_flag == false)
    {
        display_menu_options();
        option = VoiceRecognition::timed_listening_recognition_for_options();

        /* @ToDo - make sure that option can have only 3 values "ONE"/"TWO"/"UNKOWN" */
        if(strcmp("UNKOWN", option.c_str()) != 0)
        {
            if(strcmp("ONE", option.c_str()) == 0)
            {
                display_option1_message();
                option_flag = true;

                /* Start executing in paralell the Line Following, Camera and RFID Reader Communication Tasks */
                thread line_follower_thread(TASK_LINE_FOLLOWING);
                thread camera_thread(TASK_CAMERA_MODULE);
                thread reader_comm_thread(TASK_RFID_READER_COMM);
                line_follower_thread.join();
                camera_thread.join();
                reader_comm_thread.join();
            }
            else 
            {
                display_option2_message();
                option_flag = true;
            }
        }

    }


    


    

    







    terminate_main_app();

    return 0;
}
