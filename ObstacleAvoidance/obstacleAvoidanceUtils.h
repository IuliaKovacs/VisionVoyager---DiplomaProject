#ifndef OBSTACLE_AVOIDANCE
#define OBSTACLE_AVOIDANCE

#include "../ApplicationModule/application_utils.h"
#include "../VisionVoyagerMoves/visionVoyager.h"
#include <chrono>
#include <fstream>
#include <vector>
#include <string>

#define DANGER_DISTANCE_THRESHOLD 40


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
    

public:

    static void set_robot(VisionVoyager* robot);
    static void get_ultrasonic_data();
    static void avoid_simple_obstacle_right_side();
    static void avoid_simple_obstacle_left_side();
    /* Function that should compute the moves in order to return back on the route we want to pursue */
    static void return_on_track();
    /* The robot should turn back on the same route in case it fails to avoid the obstacle -> it must arrive at the starting point, exactly as it started the avoiding process */
    static void reverse_route(string route_name);
    static void simulate_real_case();
    static Direction choose_avoiding_side();
    
};



#endif //OBSTACLE_AVOIDANCE