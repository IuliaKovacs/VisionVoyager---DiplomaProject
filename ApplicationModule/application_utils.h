#ifndef APPLICATION_UTILS_H
#define APPLICATION_UTILS_H

#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <condition_variable>


#define LOGS_PATH "../VisionVoyagerGUI/DataCollection/Logs/"
#define LOG_THREAD_RFID_PREFIX "[Thread][RFID Reader]"
#define LOG_THREAD_VOICE_PREFIX "[Thread][VoiceRecognition]"
#define LOG_THREAD_LINE_FOLLOWER_PREFIX "[Thread][Line Follower]"
#define LOG_THREAD_CAMERA_PREFIX "[Thread][Camera]"
#define LOG_THREAD_ROUTE_PLAYER_PREFIX "[Thread][Route Player]"
#define LOG_THREAD_ADMIN_PREFIX "[Thread][Admin Mode]"


using namespace std;

extern ofstream logFile;


extern mutex mtx;
extern mutex log_mutex;
extern condition_variable cond_v;
extern atomic<bool> should_stop; 
extern atomic<bool> route_complete;
extern atomic<bool> severe_error;  


string log_time();

#endif //APPLICATION_UTILS_H