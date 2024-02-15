#ifndef OBSTACLE_AVOIDANCE
#define OBSTACLE_AVOIDANCE

#include "../VisionVoyagerMoves/visionVoyager.h"
#include <thread>
#include <chrono>

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

public:

    static void setRobot(VisionVoyager* robot);
    static void get_ultrasonic_data();
    static void avoid_simple_obstacle();
    static void reverse_route(string route_name);

};



#endif //OBSTACLE_AVOIDANCE