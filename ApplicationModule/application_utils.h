#ifndef APPLICATION_UTILS_H
#define APPLICATION_UTILS_H

#include <iostream>
#include <fstream>
#include <thread>

#define LOGS_PATH "../VisionVoyagerGUI/DataCollection/Logs/"

using namespace std;

extern ofstream logFile;
string log_time();

#endif //APPLICATION_UTILS_H