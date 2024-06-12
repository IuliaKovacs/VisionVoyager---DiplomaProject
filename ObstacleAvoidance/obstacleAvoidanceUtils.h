#ifndef OBSTACLE_AVOIDANCE
#define OBSTACLE_AVOIDANCE

#include "../ApplicationModule/application_utils.h"
#include "../VisionVoyagerMoves/visionVoyager.h"
#include <chrono>
#include <fstream>
#include <vector>
#include <string>

#define DANGER_DISTANCE_THRESHOLD 35
#define SEVERE_DISTANCE_THRESHOLD 15


enum class Direction
{
    LEFT,
    RIGHT,
    UNKNOWN
};


using namespace std;

class ObstacleAvoidance
{
    static VisionVoyager* robotVisionVoyager;
    static float ultrasonic_data;
    static std::chrono::time_point<std::chrono::steady_clock> start_tts_avoid;
    static Direction choose_avoiding_side();

public:

    static void set_robot(VisionVoyager* robot);
    static bool check_forward_safety();
    static std::chrono::duration<double> obstacle_avoid();
    static void get_ultrasonic_data();
    /* Function that should compute the moves in order to return back on the route we want to pursue */
    static void return_on_track();
    /* The robot should turn back on the same route in case it fails to avoid the obstacle -> it must arrive at the starting point, exactly as it started the avoiding process */
    static void reverse_route(string route_name);
};



#endif //OBSTACLE_AVOIDANCE