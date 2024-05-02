#ifndef APPLICATION_H
#define APPLICATION_H

#include "../VisionVoyagerMoves/visionVoyager.h"
#include "../RouteRegistration/routeRegistrationUtils.h"
#include "../RouteRecordPlayer/routeRecordPlayUtils.h"
#include "../KeyboardControl/keyboardControl.h"
#include "../LineFollower/lineFollower.h"
#include "../ObstacleAvoidance/obstacleAvoidanceUtils.h"
#include "../CameraModule/cameraModule.h"
#include "../VoiceRecognition/voiceRecognition.h"
#include "../TextToSpeach/textToSpeach.h"
#include "application_utils.h"
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>

using namespace std;

extern ofstream logFile;

enum class ApplicationState 
{
    STANDBY,
    MENU_DISPLAY,
    LINE_FOLLOWING,
    ROUTE_PLAYING,
    ADMIN_MODE
};


bool TASK_LINE_FOLLOWING();
bool TASK_ROUTE_PLAYING();
bool TASK_CAMERA_MODULE();
bool TASK_RFID_READER_COMM();
bool TASK_ADMIN_MODE_WINDOW(int argc, char *argv[]);



#endif //APPLICATION_H