#ifndef ROUTE_RECORD_PLAY_UTILS
#define ROUTE_RECORD_PLAY_UTILS

#include <string>
#include <optional>
#include "../VisionVoyagerMoves/visionVoyager.h"
#include "../ObstacleAvoidance/obstacleAvoidanceUtils.h"

using namespace std;

#define ROUTE_DIR_PATH "../RouteDatabase/"

class RouteRecordPlayer
{   
    static VisionVoyager* robotVisionVoyager;
    static string extract_command_name(string command);
    static string extract_command_argument(string command);
    static void play_command(string command_name, optional<int> arg_value = nullopt);

public:
    static void setRobot(VisionVoyager* robot);
    static void play_route(string route_name);  
	friend void ObstacleAvoidance::return_on_track();
};

#endif //ROUTE_RECORD_PLAY_UTILS