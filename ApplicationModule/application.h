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
#include "../TextToSpeech/textToSpeech.h"
#include "application_utils.h"
#include <optional>
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


class ApplicationModule
{
public:
    static bool TASK_LINE_FOLLOWING();
    static bool TASK_ROUTE_PLAYING(string route_name);
    static void TASK_CAMERA_MODULE();
    static bool TASK_RFID_READER_COMM(optional<string> route_name);
    static bool TASK_ADMIN_MODE_WINDOW(int argc, char *argv[]);
    static bool TASK_VOICE_RECOGNITION_WAIT();
    static bool TASK_SAFETY_MEASURES();
    static bool TASK_SPEAKING();
    static void increment_excel_route_count(string route_path);

};

#endif //APPLICATION_H