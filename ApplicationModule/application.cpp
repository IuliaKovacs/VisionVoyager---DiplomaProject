#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include "../VisionVoyagerGUI/adminModeWindow.h"
#include "application.h"
#include "../RFID/RFID_Manager.h"
#include <chrono>
#include <ctime>

using namespace std;

bool ApplicationModule::TASK_LINE_FOLLOWING()
{
    LineFollower::follow_line();
    return true;
}

bool ApplicationModule::TASK_CAMERA_MODULE()
{
    for (int i = 0; i < 10; ++i) {
    CameraModule::capture_image("../CameraModule/CapturedImages");
    /* process image*/
    std::time_t currentTime = std::time(nullptr); 
    logFile << log_time() << " -- 2 -- Thread TASK_CAMERA_MODULE processing image at datetime: " << std::ctime(&currentTime) << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
    }

    return true;
}

bool ApplicationModule::TASK_RFID_READER_COMM(optional<string> route_name)
{
    uint8_t out_data;
    RFID_request_status_t st;
    RFID_Tag_Information tag_info;
    bool system_init_status;

    RFID_init();

    st = RFID_Send_Ping();
    
    if(st == RFID_REQUEST_OK)
    {
        while(!route_complete.load())
        {
            cout << " RFID Thread " << endl;

            st = RFID_get_System_Init_Status(&system_init_status);
            if((st == RFID_REQUEST_OK) && (system_init_status == true))
            {
                st = RFID_get_Rooms(&tag_info);
                logFile << log_time() <<  "Room Request Status = " << st << endl;

                if(st == RFID_REQUEST_OK)
                {
                    logFile << log_time() << "Room name: " << tag_info.room_name << endl;
                    logFile << log_time() << "Room description: " << tag_info.room_description << endl;
                    logFile << log_time() << "Room destination_status: " << tag_info.destination_node << endl;
                }
            }
        }
    }


    return true;
}


bool ApplicationModule::TASK_ROUTE_PLAYING(string route_name)
{
     RouteRecordPlayer::play_route_conditioned(route_name);
     return true;
}

bool ApplicationModule::TASK_ADMIN_MODE_WINDOW(int argc, char *argv[]) 
{
    start_GUI(argc, argv);
    return true;
}

bool ApplicationModule::TASK_VOICE_RECOGNITION_WAIT()
{
    log_mutex.lock();
    logFile << log_time() << "[Thread][VoiceRecognition] Voice Regognition Thread Started " << endl;
    log_mutex.unlock();

    while(!route_complete.load())
    {   
        /* Listening in case the user input is "wait" */
        VoiceRecognition::loop_listening_for_wait();
        /* Check if route is already completely pursued */
        if (!route_complete.load())
        {
            /* Listening for the keyword "start" in order to continue the guidigng */
            VoiceRecognition::loop_recognition();
        }
    }

    return true;
}