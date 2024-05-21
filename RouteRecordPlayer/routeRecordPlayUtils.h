#ifndef ROUTE_RECORD_PLAY_UTILS
#define ROUTE_RECORD_PLAY_UTILS

#include <string>
#include <optional>
#include "../VisionVoyagerMoves/visionVoyager.h"
#include "../ObstacleAvoidance/obstacleAvoidanceUtils.h"

using namespace std;

#define ROUTE_DIR_PATH "../RouteDatabase/"

enum class Building_Section
{
    SECTION_A,
    SECTION_B,
    SECTION_C,
    UNKNOWN
};

class RouteRecordPlayer
{   
    static VisionVoyager* robotVisionVoyager;
    static Building_Section current_section;
    static string extract_command_name(string command);
    static string extract_command_argument(string command);
    static void play_command(string command_name, int miliseconds, optional<int> arg_value = nullopt);

public:
    static void set_robot(VisionVoyager* robot);
    static void set_current_section(Building_Section section);
    static void play_route(string route_name);
    static void play_route_conditioned(string route_name);
    static void check_hall_sensors();
    friend void ObstacleAvoidance::return_on_track();
    friend void ObstacleAvoidance::reverse_route(string route_name);
};

#endif //ROUTE_RECORD_PLAY_UTILS