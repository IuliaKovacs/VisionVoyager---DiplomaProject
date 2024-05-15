#ifndef APPLICATION_UTILS_H
#define APPLICATION_UTILS_H

#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <condition_variable>


#define LOGS_PATH "../VisionVoyagerGUI/DataCollection/Logs/"

using namespace std;

extern ofstream logFile;


extern mutex mtx;
extern mutex log_mutex;
extern condition_variable cond_v;
extern atomic<bool> should_stop; 
extern atomic<bool> route_complete; 


string log_time();

#endif //APPLICATION_UTILS_H