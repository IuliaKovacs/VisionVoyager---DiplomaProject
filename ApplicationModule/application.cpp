#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include "../VisionVoyagerGUI/adminModeWindow.h"
#include "application.h"
#include <chrono>
#include <ctime>


using namespace std;


bool TASK_LINE_FOLLOWING()
{
    LineFollower::follow_line();
}

bool TASK_CAMERA_MODULE()
{
    for (int i = 0; i < 10; ++i) {
    CameraModule::capture_image("../CameraModule/CapturedImages");
    /* process image*/
    std::time_t currentTime = std::time(nullptr); 
    logFile << log_time() << " -- 2 -- Thread TASK_CAMERA_MODULE processing image at datetime: " << std::ctime(&currentTime) << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
    }
}

bool TASK_RFID_READER_COMM()
{
    for (int i = 0; i < 10; ++i) {
        std::time_t currentTime = std::time(nullptr); 
        logFile << log_time() << " -- 3 -- Thread TASK_RFID_READER_COMM running at datetime: " << std::ctime(&currentTime) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 
    }
}


bool TASK_ROUTE_PLAYING(string route_name)
{
     RouteRecordPlayer::play_route_conditioned(route_name);
}

bool TASK_ADMIN_MODE_WINDOW(int argc, char *argv[]) 
{
    start_GUI(argc, argv);
}

bool TASK_VOICE_RECOGNITION_WAIT()
{
    logFile << log_time() << "[Thread][VoiceRecognition] Voice Regognition Thread Started " << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); 
    // Setăm flagul de oprire la true
    should_stop.store(true);
    cond_v.notify_all();
    /* in partea asta de cod ar trebui sa pun conditia de trezire - commanda START */
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); 
    // Setăm flagul inapoi
    should_stop.store(false);
    cond_v.notify_all();
    // VoiceRecognition::loop_listening_for_wait();
}