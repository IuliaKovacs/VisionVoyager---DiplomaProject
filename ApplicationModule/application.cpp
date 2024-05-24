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

    logFile << log_time() <<  LOG_RFID_PREFIX << " Start of RFID Reader Communication Thread " << endl;

    RFID_init();

    st = RFID_Send_Ping();
    
    if(st == RFID_REQUEST_OK)
    {   
        char last_room_name[20] = ""; //@ToDo

        st = RFID_get_System_Init_Status(&system_init_status);
        if((st == RFID_REQUEST_OK) && (system_init_status == true))
        {   
            st = RFID_get_Rooms(&tag_info);
            logFile << log_time() <<  LOG_RFID_PREFIX << " Room Request Status = " << st << endl;

            if(st == RFID_REQUEST_OK)
            {   
                if(strcmp(last_room_name, tag_info.room_name) != 0)
                {   
                    string room_description_message;
                    logFile << log_time() << LOG_RFID_PREFIX << " Room name: " << tag_info.room_name << endl;
                    logFile << log_time() << LOG_RFID_PREFIX << " Room description: " << tag_info.room_description << endl;
                    logFile << log_time() << LOG_RFID_PREFIX << " Room destination_status: " << tag_info.destination_node << endl;
                    if(Language::EN == TextToSpeech::get_language())
                    {
                        if(false == tag_info.destination_node)
                        {
                            room_description_message += "We pass by " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                            room_description_message += "Use the waiting command to stop here! \n\n\n\n\n\n";
                        }
                        else
                        {   
                            room_description_message += "We reached " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                            room_description_message += "This is your destination! \n\n\n\n\n\n";
                        }
                        room_description_message += string(tag_info.room_description) + "\n\n\n\n\n\n";
                    }
                    else
                    {
                        if(false == tag_info.destination_node)
                        {
                            room_description_message += "Trecem pe lângă " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                            room_description_message += "Folosește comanda de așteptare dacă dorești să ne oprim aici! \n\n\n\n\n\n";
                        }
                        else
                        {   
                            room_description_message += "Am ajuns la: " + string(tag_info.room_name) + "\n\n\n\n\n\n";
                            room_description_message += "Aceasta este destinația ta! \n\n\n\n\n\n";
                        }
                        room_description_message += string(tag_info.room_description) + "\n\n\n\n\n\n";
                    }
                    strcpy(last_room_name, tag_info.room_name);
                    TextToSpeech::display_custom_message(room_description_message);
                }

                string tag_info_route_name = string(tag_info.room_name);
                if(route_name.has_value())
                {
                    if((tag_info_route_name == RouteRegistration::get_route_name_from_path(route_name.value())) && (true == tag_info.destination_node))
                    {
                        route_complete.store(true);
                    }
                }
            }
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    }
    else
    {
        return false;
    }

    // logFile << log_time() << LOG_RFID_PREFIX << " Ended successfully! " << endl;
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
    logFile << log_time() << LOG_THREAD_VOICE_PREFIX << " Voice Regognition Thread Started " << endl;
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

bool ApplicationModule::TASK_SAFETY_MEASURES()
{
     RouteRecordPlayer::check_motors_feedback();
     return true;
}