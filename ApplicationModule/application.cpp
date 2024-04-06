#include <iostream>
#include "application.h"
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;

bool TASK_LINE_FOLLOWING()
{
    for (int i = 0; i < 10; ++i) {
        std::time_t currentTime = std::time(nullptr); 
        cout << " -- 1 -- Thread TASK_LINE_FOLLOWING running at datetime: " << std::ctime(&currentTime) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); 
    }
}

bool TASK_CAMERA_MODULE()
{
    for (int i = 0; i < 10; ++i) {
        std::time_t currentTime = std::time(nullptr); 
        cout << " -- 2 -- Thread TASK_CAMERA_MODULE running at datetime: " << std::ctime(&currentTime) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); 
    }
}

bool TASK_RFID_READER_COMM()
{
    for (int i = 0; i < 10; ++i) {
        std::time_t currentTime = std::time(nullptr); 
        cout << " -- 3 -- Thread TASK_RFID_READER_COMM running at datetime: " << std::ctime(&currentTime) << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10000)); 
    }
}