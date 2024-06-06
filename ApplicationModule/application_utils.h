#ifndef APPLICATION_UTILS_H
#define APPLICATION_UTILS_H

#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <condition_variable>


#define ROUTE_DATA_EXCEL_PATH "../VisionVoyagerGUI/DataCollection/RoutesData.xlsx"
#define SHEET_ROUTE_NAME "RoutesData"


#define LOGS_PATH "../VisionVoyagerGUI/DataCollection/Logs/"
#define LOG_THREAD_VOICE_PREFIX "[Thread][VoiceRecognition]"
#define LOG_THREAD_LINE_FOLLOWER_PREFIX "[Thread][Line Follower]"
#define LOG_THREAD_CAMERA_PREFIX "[Thread][Camera]"
#define LOG_THREAD_ROUTE_PLAYER_PREFIX "[Thread][Route Player]"
#define LOG_THREAD_ADMIN_PREFIX "[Thread][Admin Mode]"
#define LOG_THREAD_SPEAK_PREFIX "[Thread][TTS Module]"

#define LOG_RFID_PREFIX "[RFID Reader]"
#define LOG_HALL_SENSORS_PREFIX "[Hall Sensors]"


using namespace std;

enum class SevereErrorType 
{   
    NO_ERROR,
    MOTOR_ERROR,
    VOICE_RECOGNITION_ERROR,
    GPIO_ERROR,
    IN_AIR,
    LOW_VOLTAGE,
    CAMERA_ERROR
};

enum class GuidingMode 
{   
    NO_GUIDING,
    ROUTE_PLAYER_MODE,
    LINE_FOLLOWER_MODE
};

extern ofstream logFile;
extern mutex mtx;
extern mutex log_mutex;
extern mutex tts_mutex;
extern mutex speak_mutex;
extern mutex camera_mutex;
extern condition_variable waiting_condition;
extern atomic<bool> should_stop; 
extern atomic<bool> route_complete;
extern atomic<bool> severe_error;  
extern atomic<bool> moving;
extern SevereErrorType error_type;
extern condition_variable speaking_condition;
extern condition_variable camera_condition;
extern atomic<bool> speak;
extern string global_message;
extern GuidingMode guiding_mode;

string log_time();

#endif //APPLICATION_UTILS_H