#ifndef OBSTACLE_AVOIDANCE
#define OBSTACLE_AVOIDANCE

#include "../VisionVoyagerMoves/visionVoyager.h"
#include <thread>
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
    // static Direction choose_avoiding_side(); 
    static void reverse_route(string route_name);

public:

    static void setRobot(VisionVoyager* robot);
    static void get_ultrasonic_data();
    static void avoid_simple_obstacle_right_side();
    static void avoid_simple_obstacle_left_side();
	/* Function that should compute the moves in order to return back on the route we want to pursue */
    static void return_on_track();
    

};



#endif //OBSTACLE_AVOIDANCE